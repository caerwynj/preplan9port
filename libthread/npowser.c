#include <u.h>
#include <libc.h>
#include <thread.h>
#include <threadassert.h>

#define STACKSIZE (512*1024)

void *params[10];

/*
 * Power series package
 * A power series is a channel, along which flow rational
 * coefficients.  A denominator of zero signifies the end.
 */

typedef struct Rat {
	int num, den;	/* numerator, denominator */
} Rat;

typedef Rat Item;

struct Channel {
	int	s;		/* Size of the channel (may be zero) */
	uint	f;		/* Extraction point (insertion pt: (f + n) % s) */
	uint	n;		/* Number of values in the channel */
	int	e;		/* Element size */
	int	freed;		/* Set when channel is being deleted */
	ulong	qused;		/* Bitmap of used entries in rcvrs */
	Alt	*qentry[32];	/* Receivers/senders waiting */
	uchar	v[1];		/* Array of max(1, s) values in the channel */
};

int recvrat(Channel *c, Rat *v) {
	int r;

	threadassert(c->e == sizeof(Rat));	/* type check */
	r = recv(c, v);
	threadassert(r >= 0);
	_threaddebug(DBGAPPL, "Recv on channel %lux: %d/%d",
		c, ((Rat *)v)->num, ((Rat *)v)->den);
	return r;
}

int sendrat(Channel *c, Rat *v) {
	int r;

	threadassert(c->e == sizeof(Rat));	/* type check */
	_threaddebug(DBGAPPL, "Send on channel %lux: %d/%d",
		c, ((Rat *)v)->num, ((Rat *)v)->den);
	r = send(c, v);
	threadassert(r >= 0);
	return r;
}

/*
 * Dch is a "demand channel".  The producer will not
 * begin computing a datum until the consumer has sent
 * a request for one.  Requests come into .req, values are
 * sent back to the requester on .dat.
 */

/*
 * For _threaddebugging each channel contains an identifying .nam field,
 * and requests are assigned sequence numbers.  No attempt
 * is made to eliminate races in maintaining _threaddebug info
 */

typedef struct Dch {
	Channel	*req;	/* chan of int */
	Channel	*dat;	/* chan of item */
	char	name; 
} Dch;

char	*chnames = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int		chnameserial = -1;

Dch *
mkdch(void) {
	Dch *dch;

	dch = (Dch *)malloc(sizeof(Dch));
	threadassert(dch != nil);

	chnameserial = (chnameserial+1) % strlen(chnames);
	dch->req = chancreate(sizeof(int), 0);
	threadassert(dch->req != nil);
	dch->dat = chancreate(sizeof(Item), 0);
	threadassert(dch->dat != nil);
	dch->name = chnames[chnameserial];

	return dch;
}

Dch *
mkdch2(void) {
	Dch *dch;

	dch = (Dch *)malloc(2*sizeof(Dch));
	threadassert(dch != nil);

	chnameserial = (chnameserial+1) % strlen(chnames);
	dch[0].req = chancreate(sizeof(int), 0);
	threadassert(dch[0].req != nil);
	dch[0].dat = chancreate(sizeof(Item), 0);
	threadassert(dch[0].dat != nil);
	dch[0].name = chnames[chnameserial];

	chnameserial = (chnameserial+1) % strlen(chnames);
	dch[1].req = chancreate(sizeof(int), 0);
	threadassert(dch[1].req != nil);
	dch[1].dat = chancreate(sizeof(Item), 0);
	threadassert(dch[1].dat != nil);
	dch[1].name = chnames[chnameserial];

	return dch;
}

ulong seqno = 0;

/*
 * split reads a single demand channel and replicates its
 * output onto two, which may be read at different rates.
 * A process is created at first demand for an item and dies
 * after the item has been sent to both outputs.
 */

/*
 * When multiple generations of split exist, the newest
 * will service requests on one channel, which is
 * always renamed to be out[0]; the oldest will service
 * requests on the other channel, out[1].  All generations but the
 * newest hold queued data that has already been sent to
 * out[0].  When data has finally been sent to out[1],
 * a signal on the release-wait channel tells the next newer
 * generation to begin servicing out[1].
 */

void
dosplit(void *inparam) {
	Dch *in; Dch out[2]; Channel /*of int*/ *wait;
	int both, r;
	long *outparam;
	Alt a[3];
	int v0, v1;
	Channel *release;
	Item dat;

	in = (Dch*)((long*)inparam)[0];
	memcpy(out, (char*)((long*)inparam)[1], 2*sizeof(Dch));
	wait = (Channel*)((long*)inparam)[2];
	free(inparam);
	threadsetname("Dosplit");

	_threaddebug(DBGAPPL, "dosplit(%lux, %lux, %lux)", in, out, wait);

	a[0].c = out[0].req;	a[1].c = wait;
	a[0].op = CHANRCV;	a[1].op = CHANRCV;	a[2].op = CHANEND;
	a[0].v = &v0;		a[1].v = &v1;

	both = 0; /* do not service both channels */
	switch(r = alt(a)) {
	default:
		return;
	case 0:
		break;
	case 1:
		both = 1;
		a[0].c = out[0].req;	a[1].c = out[1].req;
		switch(r = alt(a)) {
			Dch t;
		default:
			return;
		case 0:
			break;
		case 1:
			memcpy(&t, &out[0], sizeof(Dch));
			memcpy(&out[0], &out[1], sizeof(Dch));
			memcpy(&out[1], &t, sizeof(Dch));
			break;
		}
		break;
	}
	_threaddebug(DBGAPPL, "dosplit");
	sendul(in->req, ++seqno);
	release = chancreate(sizeof(int), 0);
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)in;
	outparam[1] = (long)out;
	outparam[2] = (long)release;
	threadcreate(dosplit, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
	recvrat(in->dat, &dat);
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
	sendrat(out[0].dat, &dat);
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
	if(both==0) recvul(wait);
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
	recvul(out[1].req);
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
	sendrat(out[1].dat, &dat);
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
	sendul(release, 0);
	_threaddebug(DBGAPPL, "dosplit %d", __LINE__);
}

void
splitp(void *inparam) {
/* Params: in:dch, out:dch2 */
	Dch *in, *out;
	Channel *release;
	long *outparam;

	in = (Dch*)((long *)inparam)[0];
	out = (Dch*)((long *)inparam)[1];
	free(inparam);
	threadsetname("splitp");

	_threaddebug(DBGAPPL, "splitp(%lux, %lux)", in, out);

	release = chancreate(sizeof(int), 0);
	outparam = malloc(sizeof(long *) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)in;
	outparam[1] = (long)out;
	outparam[2] = (long)release;
	proccreate(dosplit, (void *)outparam, STACKSIZE);
	yield(); /* run new thread first */
	sendul(release, 0);
}

void
split(Dch *in, Dch *out) {
	Channel *release;
	long *outparam;

	_threaddebug(DBGAPPL, "split(%lux, %lux)", in, out);

	release = chancreate(sizeof(int), 0);
	outparam = malloc(sizeof(long *) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)in;
	outparam[1] = (long)out;
	outparam[2] = (long)release;
	threadcreate(dosplit, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	sendul(release, 0);
}

void
put(Item *dat, Dch *out) {

	_threaddebug(DBGAPPL, "put(%d/%d, %lux)", dat->num, dat->den, out);

	recvul(out->req);
	sendrat(out->dat, dat);
}

void
get(Dch *in, Item *i) {

	_threaddebug(DBGAPPL, "get(%lux, i)", in);

	sendul(in->req, ++seqno);
	recvrat(in->dat, i);
}


#define REQ0 1
#define DAT0 2
#define REQ1 4
#define DAT1 8

/*
 * Get one item from each of 2 demand channels
 */
void
get2(Dch *in0, Dch *in1, Item *out) {
	Alt a[5];
	int s = 0;
	int r;

	_threaddebug(DBGAPPL, "get2(%lux, %lux, out)", in0, in1);

	a[0].c = in0->req;
	a[0].v = &seqno;
	a[1].c = in1->req;
	a[1].v = &seqno;
	a[2].c = in0->dat;
	a[2].v = &out[0];
	a[3].c = in1->dat;
	a[3].v = &out[1];
	a[4].op = CHANEND;

	while(s != (REQ0|REQ1|DAT0|DAT1)) {
		seqno++;
		switch (s) {
		case 0:
			a[0].op = CHANSND;
			a[1].op = CHANSND;
			a[2].op = CHANNOP;
			a[3].op = CHANNOP;
			switch (r = alt(a)) {
			default:
				_threaddebug(DBGAPPL, "%d", r);
				threadassert(0);
			case 0:
				s |= REQ0;
				break;
			case 1:
				s |= REQ1;
				break;
			}
			break;
		case REQ0:
			a[0].op = CHANNOP;
			a[1].op = CHANSND;
			a[2].op = CHANRCV;
			a[3].op = CHANNOP;
			switch (r = alt(a)) {
			default:
				_threaddebug(DBGAPPL, "%d", r);
				threadassert(0);
			case 0:
				s |= REQ0;
				break;
			case 1:
				s |= REQ1;
				break;
			case 2:
				s |= DAT0;
				break;
			case 3:
				s |= DAT1;
				break;
			}
			break;
		case REQ1:
			a[0].op = CHANSND;
			a[1].op = CHANNOP;
			a[2].op = CHANNOP;
			a[3].op = CHANRCV;
			switch (r = alt(a)) {
			default:
				_threaddebug(DBGAPPL, "%d", r);
				threadassert(0);
			case 0:
				s |= REQ0;
				break;
			case 1:
				s |= REQ1;
				break;
			case 2:
				s |= DAT0;
				break;
			case 3:
				s |= DAT1;
				break;
			}
			break;
		case REQ0|REQ1:
			a[0].op = CHANNOP;
			a[1].op = CHANNOP;
			a[2].op = CHANRCV;
			a[3].op = CHANRCV;
			switch (r = alt(a)) {
			default:
				_threaddebug(DBGAPPL, "%d", r);
				threadassert(0);
			case 0:
				s |= REQ0;
				break;
			case 1:
				s |= REQ1;
				break;
			case 2:
				s |= DAT0;
				break;
			case 3:
				s |= DAT1;
				break;
			}
			break;
		case REQ0|DAT0:
			sendul(in1->req, seqno);
			s |= REQ1;
			break;
		case REQ1|DAT1:
			sendul(in0->req, seqno);
			s |= REQ0;
			break;
		case REQ0|REQ1|DAT0:
			recvrat(in1->dat, &out[1]);
			s |= DAT1;
			break;
		case REQ0|REQ1|DAT1:
			recvrat(in0->dat, &out[0]);
			s |= DAT0;
			break;
		default:
			threadassert(0);
		}
	}
}

 
void
copy(Dch *in, Dch *out) {
	Item i;

	_threaddebug(DBGAPPL, "copy(%lux, %lux)", in, out);

	for(;;) {
		recvul(out->req);
		get(in, &i);
		sendrat(out->dat, &i);
	}
}

void
repeat(void *inparam) {
	Item dat; Dch *out;

	dat = *(Item *)((long *)inparam)[0];
	out = (Dch *)((long *)inparam)[1];
	free(inparam);

	threadsetname("Repeat");
	_threaddebug(DBGAPPL, "repeat(%d/%d, %lux)", dat.num, dat.den, out);

	for(;;)
		put(&dat, out);
}

typedef Dch PS;	/* power series */
typedef Dch PS2; /* pair of power series */

/*
 * Conventions
 * Upper-case for power series.
 * Lower-case for rationals.
 * Input variables: U,V,...
 * Output variables: ...,Y,Z
 */

/*
 * Integer gcd; needed for rational arithmetic
 */

int
gcd(int u, int v) {
	if(u<0) 
		return gcd(-u,v);
	if(u>v)
		return gcd(v,u);
	if(u==0)
		return v;
	return gcd(v%u,u);
}

/*
 * Make a rational from two ints and from one int
 */

void
i2tor(int u, int v, Rat *i) {
	int g;

	g = gcd(u,v);
	if(v >= 0) {
		i->num = u/g;
		i->den = v/g;
	} else {
		i->num = -u/g;
		i->den = -v/g;
	}
}

void
itor(int u, Rat *i) {
	i2tor(u, 1, i);
}

Rat zero, one;

/*
 * End mark and end test
 */
Rat finis;

int
eend(Rat *u) {
	return u->den==0;
}

/*
 * Operations on rationals
 */

void
add(Rat *u, Rat *v, Rat *r) {
	int g;

	g = gcd(u->den, v->den);
	i2tor(u->num*(v->den/g)+v->num*(u->den/g), u->den*(v->den/g), r);
}

void
mul(Rat *u, Rat *v, Rat *r) {
	int g1, g2;

	g1 = gcd(u->num, v->den);
	g2 = gcd(u->den, v->num);
	r->num = (u->num/g1)*(v->num/g2);
	r->den = (u->den/g2)*(v->den/g1);
}

void
neg(Rat *u, Rat *r) {
	r->num = -u->num;
	r->den = u->den;
}

void
sub(Rat *u, Rat *v, Rat *r) {
	Rat x;

	neg(v, &x);
	add(u, &x, r);
}

void
inv(Rat *u, Rat *r) {	/* invert a rat */
	int x;

	x = 1/u->num;	/* induce a stop on zero-divide */
	i2tor(u->den, u->num, r);
}

void
ratprint(Rat *u) {
	if (u->den==1) print("%d", u->num);
	else print("%d/%d", u->num, u->den);
	print(" ");
}

/*
 * Print n terms of a power series
 */

void
Printn(PS *U, int n) {
	int done;

	_threaddebug(DBGAPPL, "Printn(%lux, %d)", U, n);

	done = 0;
	for(; !done && n>0; n--) {
		Rat u;

		get(U, &u);
		if (eend(&u)) done=1;
		else ratprint(&u);
	}
	print("\n");
}

void
Print(PS *U){
	Printn(U, 1000000000);
}

/*
 * Evaluate n terms of power series U at x=c
 */

void
eval(Rat *c, PS *U, int n, Rat *r) {
	Rat x, y, z;

	if (n==0) {
		r->num = 0;
		r->den = 1;
		return;
	}
	get(U, &y);
	if (eend(&y)) {
		r->num = 0;
		r->den = 1;
		return;
	}
	eval(c, U, n-1, &x);
	mul(c, &x, &z);
	add(&y, &z, r);
}

/*
 * Power-series constructors return channels on which power
 * series flow.  They start an encapsulated generator that
 * puts the terms of the series on the channel.
 */

/*
 * Make a pair of power series identical to a given power series
 */

Dch *
Split(PS *U) {
	long *outparam;
	Dch *UU;

	_threaddebug(DBGAPPL, "Split(%lux)", U);

	UU = mkdch2();

	outparam = malloc(sizeof(long) * 2);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)UU;
	threadcreate(splitp, outparam, STACKSIZE);
	yield(); /* run new thread first */
	return UU;
}

/*
 * Add two power series
 */

void
Addp(void *inparam) {
	Item uv[2], t;
	PS *U, *V, *Z;

	U = (PS*)((long *)inparam)[0];
	V = (PS*)((long *)inparam)[1];
	Z = (PS*)((long *)inparam)[2];
	free(inparam);

	threadsetname("Addp");
	_threaddebug(DBGAPPL, "Addp(%lux, %lux, %lux)", U, V, Z);

	for(;;) {
		recvul(Z->req);
		get2(U, V, uv);
		switch(eend(&uv[0])+2*eend(&uv[1])) {
		case 0:
			add(&uv[0], &uv[1], &t);
			sendrat(Z->dat, &t);
			break;
		case 1:
			sendrat(Z->dat, &uv[1]);
			copy(V,Z);
			return;
		case 2:
			sendrat(Z->dat, &uv[0]);
			copy(U,Z);
			return;
		case 3:
			sendrat(Z->dat, &finis);
		}
	}
}

PS *
Add(PS *U, PS *V) {
	PS *Z;
	long *outparam;

	_threaddebug(DBGAPPL, "Add(%lux, %lux)", U, V);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)V;
	outparam[2] = (long)Z;
	threadcreate(Addp, (void *)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Multiply a power series by a constant
 */

void
Cmulp(void *inparam) {
	Rat c; PS *U, *Z;
	int done;
	Item u, t;

	c = *(Rat *)((long*)inparam)[0];
	U = (PS*)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);

	threadsetname("Cmulp");
	_threaddebug(DBGAPPL, "Cmulp(%d/%d, %lux, %lux)", c.num, c.den, U, Z);

	done = 0;
	while(!done) {
		recvul(Z->req);
		get(U, &u);
		if(eend(&u)) done=1;
		else {
			mul(&c, &u, &t);
			sendrat(Z->dat, &t);
		}
	}
	sendrat(Z->dat, &finis);
}

PS *
Cmul(Rat *c, PS *U) {
	PS *Z;
	long *outparam;

	_threaddebug(DBGAPPL, "Cmul(%d/%d, %lux)", c->num, c->den, U);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)c;
	outparam[1] = (long)U;
	outparam[2] = (long)Z;
	threadcreate(Cmulp, outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Subtract
 */

PS *
Sub(PS *U, PS *V) {
	Rat r;

	_threaddebug(DBGAPPL, "Sub(%lux, %lux)", U, V);

	neg(&one, &r);
	return Add(U, Cmul(&r,V));
}

/*
 * Multiply a power series by the monomial x^n
 */

void
Monmulp(void *inparam) {
	PS *U; int n; PS *Z;

	U = (PS*)((long*)inparam)[0];
	n = (int)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);
	threadsetname("Monmulp");

	_threaddebug(DBGAPPL, "Monmulp(%lux, %d, %lux)", U, n, Z);

	for(; n>0; n=n-1) put(&zero, Z);
	copy(U,Z);
	return;
}

PS *
Monmul(PS *U, int n) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Monmul(%lux, %d)", U, n);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)n;
	outparam[2] = (long)Z;
	threadcreate(Monmulp, outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Multiply by x
 */

PS *
Xmul(PS *U) {

	_threaddebug(DBGAPPL, "Xmul(%lux)", U);

	return Monmul(U,1);
}

PS *
Rep(Rat *c) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Rep(%d/%d)", c->num, c->den);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 2);
	threadassert(outparam != nil);
	outparam[0] = (long)c;
	outparam[1] = (long)Z;
	threadcreate(repeat, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Monomial c*x^n
 */

void
Monp(void *inparam) {
	Rat c; int n; PS *Z;

	c = *(Rat *)((long*)inparam)[0];
	n = (int)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);

	threadsetname("Monp");
	_threaddebug(DBGAPPL, "Monp(%d/%d, %d, %lux)", c.num, c.den, n, Z);

	if(c.num != 0) {
		for(; n>0; n=n-1) put(&zero, Z);
		put(&c, Z);
	}
	put(&finis, Z);
}

PS *
Mon(Rat *c, int n) {
	long *outparam;
	PS *Z;

	_threaddebug(DBGAPPL, "Mon(%d/%d, %d)", c->num, c->den, n);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long) c;
	outparam[1] = (long) n;
	outparam[2] = (long) Z;
	threadcreate(Monp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

void
Shiftp(void *inparam) {
	Rat c; PS *U; PS *Z;

	c = *(Rat *)((long*)inparam)[0];
	U = (PS*)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);

	threadsetname("Shiftp");
	_threaddebug(DBGAPPL, "Shiftp(%d/%d, %lux, %lux)", c.num, c.den, U, Z);

	put(&c,Z);
	copy(U,Z);
}

PS *
Shift(Rat *c, PS *U) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Shift(%d/%d, %lux)", c->num, c->den, U);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)c;
	outparam[1] = (long)U;
	outparam[2] = (long)Z;
	threadcreate(Shiftp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * simple pole at 1: 1/(1-x) = 1 1 1 1 1 ...
 */

/*
 * Convert array of coefficients, constant term first
 * to a (finite) power series
 */

void
Polyp(void *inparam) {
	int done, i, j;
	Rat *a; int n; PS *Z;

	a = (Rat*)((long*)inparam)[0];
	n = (int)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);

	threadsetname("Polyp");
	done = 0;
	for(j = n; !done && j > 0; j = j-1)
		if (a[j-1].num != 0) done = 1;
	for(i = 0; i < j; i = i+1) put(&a[i], Z);
	put(&finis, Z);
}

PS *
Poly(Rat *a, int n) {
	long *outparam;
	Dch *Z;

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)a;
	outparam[1] = (long)n;
	outparam[2] = (long)Z;
	threadcreate(Polyp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Multiply. The algorithm is
 * 	let U = u + x*UU
 *	let V = v + x*VV
 *	then UV = u*v + x*(u*VV+v*UU) + x*x*UU*VV
 */
PS *Mul(PS *U, PS *V);

void
Mulp(void *inparam){
	PS *U, *V, *Z;
	Item uv[2];
	Dch *UU, *VV;
	PS *W;
	Item w;

	U = (PS*)((long*)inparam)[0];
	V = (PS*)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);

	_threaddebug(DBGAPPL, "Mulp(%lux, %lux, %lux)", U, V, Z);

	recvul(Z->req);
	get2(U, V, uv);
	if(eend(&uv[0])||eend(&uv[1]))
		sendrat(Z->dat, &finis);
	else {
		mul(&uv[0], &uv[1], &w);
		sendrat(Z->dat, &w);
		UU = Split(U);
		VV = Split(V);
		W  = Add(Cmul(&uv[0], &VV[0]), Cmul(&uv[1], &UU[0]));
		recvul(Z->req);
		get(W, &w);
		sendrat(Z->dat, &w);
		copy(Add(W, Mul(&UU[1], &VV[1])), Z);
	}
}

PS *
Mul(PS *U, PS *V) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Mul(%lux, %lux)", U, V);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)V;
	outparam[2] = (long)Z;
	threadcreate(Mulp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Differentiate
 */

void
Diffp(void *inparam) {
	PS *U; PS *Z;
	Item u;
	Rat r;

	U = (PS*)((long*)inparam)[0];
	Z = (PS*)((long*)inparam)[1];
	free(inparam);

	threadsetname("Diffp");
	_threaddebug(DBGAPPL, "Diffp(%lux, %lux)", U, Z);

	recvul(Z->req);
	get(U, &u);
	if(!eend(&u)) {
		int i, done;

		done=0;
		for(i=1; !done; i=i+1) {
			get(U, &u);
			if(eend(&u)) done=1;
			else {
				itor(i, &r);
				mul(&r, &u, &r);
				sendrat(Z->dat, &r);
				recvul(Z->req);
			}
		}
	}
	sendrat(Z->dat, &finis);
}

PS *
Diff(PS *U) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Diff(%lux)", U);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 2);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)Z;
	threadcreate(Diffp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Integrate, with const of integration
 */

void
Integp(void *inparam){
	Rat c; PS *U, *Z;
	int done, i;
	Item u, m;

	c = *(Rat *)((long*)inparam)[0];
	U = (PS*)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);

	threadsetname("Integp");
	_threaddebug(DBGAPPL, "Integp(%d/%d, %lux, %lux)", c.num, c.den, U, Z);

	put(&c,Z);
	done = 0;
	for(i = 1; !done; i=i+1) {
		recvul(Z->req);
		get(U, &u);
		if(eend(&u)) done=1;
		i2tor(1,i, &m);
		mul(&m, &u, &m);
		sendrat(Z->dat, &m);
	}
	sendrat(Z->dat, &finis);
}

PS *
Integ(Rat *c, PS *U) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Integ(%d/%d, %lux)", c->num, c->den, U);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)c;
	outparam[1] = (long)U;
	outparam[2] = (long)Z;
	threadcreate(Integp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Binomial theorem (1+x)^c
 */

void
Binomp(void *inparam){
	Rat c; PS *Z;
	int n;
	Rat t, x, m;

	c = *(Rat *)((long*)inparam)[0];
	Z = (PS*)((long*)inparam)[1];
	free(inparam);
	threadsetname("Binomp");

	_threaddebug(DBGAPPL, "Binomp(%d/%d, %lux)", c.num, c.den, Z);

	n = 1;
	itor(1, &t);
	while(c.num != 0){
		put(&t, Z);
		mul(&t, &c, &m);
		i2tor(1, n, &x);
		mul(&m, &x, &t);
		sub(&c, &one, &c);
		n = n+1;
	}
	put(&finis, Z);
}

PS *
Binom(Rat *c) {
	long *outparam;
	Dch *Z;

	_threaddebug(DBGAPPL, "Binom(%d/%d)", c->num, c->den);

	Z = mkdch();
	outparam = malloc(sizeof(long) * 2);
	threadassert(outparam != nil);
	outparam[0] = (long)c;
	outparam[1] = (long)Z;
	threadcreate(Binomp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Reciprocal of a power series
 *	let U = u + x*UU
 *	let Z = z + x*ZZ
 *	(u+x*UU)*(z+x*ZZ) = 1
 *	z = 1/u
 *	u*ZZ + z*UU +x*UU*ZZ = 0
 *	ZZ = -UU*(z+x*ZZ)/u;
 */

void
Recipp(void *inparam){
	PS *U, *Z, *ZZ;
	Rat z, u;

	U = (PS *)((long *)inparam)[0];
	Z = (PS *)((long *)inparam)[1];
	ZZ = (PS *)((long *)inparam)[2];
	free(inparam);
	threadsetname("Recipp");

	_threaddebug(DBGAPPL, "Recipp(%lux, %lux, %lux)", U, Z, ZZ);

	recvul(Z->req);
	get(U, &u);
	inv(&u, &z);
	sendrat(Z->dat, &z);
	neg(&z, &u);
	split(Mul(Cmul(&u, U), Shift(&z, &ZZ[0])), ZZ);
	copy(&ZZ[1],Z);
}

PS *
Recip(PS *U) {
	long *outparam;
	Dch *Z, *ZZ;

	_threaddebug(DBGAPPL, "Recip(%lux)", U);

	Z = mkdch();
	ZZ = mkdch2();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)Z;
	outparam[2] = (long)ZZ;
	threadcreate(Recipp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

/*
 * Exponential of a power series with constant term 0
 * (nonzero constant term would make nonrational coefficients)
 * bug: the constant term is simply ignored
 *	Z = exp(U)
 *	DZ = Z*DU
 *	integrate to get Z
 */

PS *
Exp(PS *U) {
	Dch *ZZ;

	_threaddebug(DBGAPPL, "Exp(%lux)", U);
	ZZ = mkdch2();
	split(Integ(&one, Mul(&ZZ[0], Diff(U))), ZZ);
	return &ZZ[1];
}

/*
 * Substitute V for x in U, where the leading term of V is zero
 *	let U = u + x*UU
 *	let V = v + x*VV
 * 	then S(U,V) = u + VV*S(V,UU)
 * bug: a nonzero constant term is ignored
 */
PS *Subst(PS *U, PS *V);

void
Substp(void *inparam){
	PS *U, *V, *Z;
	Dch *VV;
	Rat u, x;

	U = (PS*)((long*)inparam)[0];
	V = (PS*)((long*)inparam)[1];
	Z = (PS*)((long*)inparam)[2];
	free(inparam);
	threadsetname("Substp");

	_threaddebug(DBGAPPL, "Subst(%lux, %lux, %lux)", U, V, Z);

	VV = Split(V);
	recvul(Z->req);
	get(U, &u);
	sendrat(Z->dat, &u);
	if(!eend(&u))
		get(&VV[0], &x);
		if(eend(&x)) put(&finis, Z);
		else copy(Mul(&VV[0],Subst(U,&VV[1])),Z);
}

PS *
Subst(PS *U, PS *V) {
	long *outparam;
	Dch *Z;

	Z = mkdch();
	outparam = malloc(sizeof(long) * 3);
	threadassert(outparam != nil);
	outparam[0] = (long)U;
	outparam[1] = (long)V;
	outparam[2] = (long)Z;
	threadcreate(Substp, (void*)outparam, STACKSIZE);
	yield(); /* run new thread first */
	return Z;
}

void
main(int argc, char *argv[]) {
	Dch *Twos;
	PS *Ones;
	Rat two, sixteen;

	threadinit();
	threadsetname("main");
	_threaddebuglevel = DBGRACE|DBGALRM;
	_threaddebug(DBGAPPL, "testproc");
	itor(0, &zero);
	itor(1, &one);
	i2tor(1,0, &finis);
	Ones = Rep(&one);
	itor(2, &two);
	Twos = Rep(&two);
	/*1 16 120 560 1820 4368 8008 11440 12870 11440  */
	itor(16, &sixteen);
	Printn(Binom(&sixteen), 10);
	/*2 34 274 1394 5034 13770 29786 52666 78406 101286  */
	Printn(Mul(Binom(&sixteen), Twos), 10);
	/*1 1 3/2 13/6 73/24 167/40 4051/720 37633/5040 43817/4480 4596553/362880 */
	Printn(Exp(Ones), 10);
	threadexitsall(nil);
}
