#include "pd.h"

#define	NSTACK	256

static Datum stack[NSTACK];	/* the stack */
static Datum *stackp;		/* next free spot on stack */

void
initcode(void)
{
	stackp = stack;
}

void
push(Datum d)
{
	if (stackp >= &stack[NSTACK])
		execerror("stack too deep", 0);
	*stackp++ = d;
}

Datum
pop(void)
{
	if (stackp == stack)
		execerror("stack underflow", 0);
	return *--stackp;
}

void
xpop(void)	/* for when no value is wanted */
{
	if (stackp == stack)
		execerror("stack underflow", (char *)0);
	--stackp;
	if(stackp->type == SSTR)
		release(stackp->p);
}

void
constpush(void)
{
	Datum d;
	d.type = SNUM;
	d.val = sym->u.val;
	push(d);
}

void
varpush(void)
{
	Datum d;
	d.type = SSYM;
	d.sym = sym;
	push(d);
}

void
strpush(void)
{
	Datum d;
	d.type = SSTR;
	d.p = (Blk*)sym;
	push(d);
}

void
whilecode(void)
{
}

void
forcode(void)
{
}

void
ifcode(void) 
{
	Datum d1, d2;

	d1 = pop();
	d2 = pop();
	if (d2.val)
		pushio(d1.p);
}

void
xec(void)
{
	Datum d;
	d = pop();
	pushio(d.p);
}

void
bltin(void) 
{

	Datum d;
	d = pop();
	d.val = (*(double (*)(double))sym->u.ptr)(d.val);
	push(d);
}

void
add(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val += d2.val;
	push(d1);
}

void
sub(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val -= d2.val;
	push(d1);
}

void
mul(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val *= d2.val;
	push(d1);
}

void
div(void)
{
	Datum d1, d2;
	d2 = pop();
	if (d2.val == 0.0)
		execerror("division by zero", (char *)0);
	d1 = pop();
	d1.val /= d2.val;
	push(d1);
}

void
mod(void)
{
	Datum d1, d2;
	d2 = pop();
	if (d2.val == 0.0)
		execerror("division by zero", (char *)0);
	d1 = pop();
	/* d1.val %= d2.val; */
	d1.val = fmod(d1.val, d2.val);
	push(d1);
}

void
negate(void)
{
	Datum d;
	d = pop();
	d.val = -d.val;
	push(d);
}

void
verify(Symbol* s)
{
	if (s->type != LVAR && s->type != LUNDEF)
		execerror("attempt to evaluate non-variable", s->name);
	if (s->type == LUNDEF)
		execerror("undefined variable", s->name);
}

void
eval(void)		/* evaluate variable on stack */
{
	Datum d;
	d = pop();
	verify(d.sym);
	d.val = d.sym->u.val;
	push(d);
}

void
gt(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val > d2.val);
	push(d1);
}

void
lt(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val < d2.val);
	push(d1);
}

void
ge(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val >= d2.val);
	push(d1);
}

void
le(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val <= d2.val);
	push(d1);
}

void
eq(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val == d2.val);
	push(d1);
}

void
ne(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != d2.val);
	push(d1);
}

void
and(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != 0.0 && d2.val != 0.0);
	push(d1);
}

void
or(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != 0.0 || d2.val != 0.0);
	push(d1);
}

void
not(void)
{
	Datum d;
	d = pop();
	d.val = (double)(d.val == 0.0);
	push(d);
}

void
power(void)
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = Pow(d1.val, d2.val);
	push(d1);
}

void
assign(void)
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != LVAR && d1.sym->type != LUNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d1.sym->u.val = d2.val;
	d1.sym->type = LVAR;
	push(d2);
}

void
printtop(void)	/* pop top value from stack, print it */
{
	Datum d;
	static Symbol *s;	/* last value computed */
	if (s == 0)
		s = install("_", LVAR, 0.0);
	d = pop();
	if(d.type == SNUM) {
		print("%.12g\n", d.val);
		s->u.val = d.val;
	} else if(d.type == SSTR) {
		rewind(d.p);
		while(sfeof(d.p) == 0) {
			print("%c",sgetc(d.p));
		}
		print("\n");
		release(d.p);
		return;
	} else if(d.type == SFSTR) {
		if(debug)
			fprint(2, "printtop SFSTR\n");
		print("%s\n", d.s);
	}
}

void
prexpr(void)	/* print numeric value */
{
	Datum d;
	d = pop();
	print("%.12g ", d.val);
}

void
varread(void)	/* read into variable */
{
	Datum d;
	extern Biobuf *bin;
	Symbol *var = sym;
	int c;

  Again:
	do
		c = Bgetc(bin);
	while(c==' ' || c=='\t');
	if(c == Beof){
  Iseof:
		if(moreinput())
			goto Again;
		d.val = var->u.val = 0.0;
		goto Return;
	}

	if(strchr("+-.0123456789", c) == 0)
		execerror("non-number read into", var->name);
	Bungetc(bin);
	if(Bgetd(bin, &var->u.val) == Beof)
		goto Iseof;
	else
		d.val = 1.0;
  Return:
	var->type = LVAR;
	push(d);
}

void
now(void)
{
	Datum d;
	d.type=SNUM;
	d.val = time(0);
	push(d);
}

void
btopen(void)
{
	Datum d;

	d = pop();
	d.sym->type = LTEA;
	if((d.sym->u.ptr = malloc(sizeof(Btree))) == 0)
		execerror("btree", "");
	btree((Btree*)d.sym->u.ptr, d.sym->name, OREAD);
}

void
xeff(void)
{
	Datum d1, d2, d3, d4;
	char *p;
	Block r = {0, 0, 0, 0, 0, 0};

	d4 = pop();
	d3 = pop();
	d2 = pop();
	d1 = pop();
	sputc(d2.p, 0);
	sputc(d3.p, 0);
	join((Btree*)d4.sym->u.ptr, (long)d1.val, d2.p->beg, d3.p->beg, 2, JSINGLE|JEXACT, &r);
	if(debug)
		fprint(2, "xeff ret %d\n", r.m);
	if(r.m == 0)
		return;
	d1.val = r.ents[0]->vt;
	push(d1);
	screate(d2.p);
	for(p = r.ents[0]->key; *p; p++)
		sputc(d2.p, *p);
	push(d2);
	screate(d3.p);
	for(p = r.ents[0]->val; *p; p++)
		sputc(d3.p, *p);
	push(d3);
}

void
load(void)
{
	Datum d1, d2, d3, d4, d5;
	Block *r;

	d5 = pop();
	d4 = pop();
	d3 = pop();
	d2 = pop();
	d1 = pop();
	sputc(d2.p, 0);
	sputc(d3.p, 0);
	r = emalloc(sizeof(Block));
	memset(r, 0, sizeof(Block));
	join((Btree*)d4.sym->u.ptr, (long)d1.val, d2.p->beg, d3.p->beg, 2, (int)d5.val, r);
	d1.type = SBLK;
	d1.b = r;
	if(debug)
		fprint(2, "result set: %d\n", d1.b->m);
	release(d3.p);
	release(d2.p);
	push(d1);
}

void
get(void)
{
	Datum d1, d2, d3, d4, d5;
	int i;

	d2 = pop();
	d1 = pop();
	i = (int) d2.val;
	if(debug)
		fprint(2, "index %d m %d\n", i, d1.b->m);
	if(i >= d1.b->m)
		return;
	d3.type = SNUM;
	d3.val = d1.b->ents[i]->vt;
	d4.type = SFSTR;
	d4.s = d1.b->ents[i]->key;
	d5.type = SFSTR;
	d5.s = d1.b->ents[i]->val;
	push(d3);
	push(d4);
	push(d5);
}

void
forall(void)
{
	Datum d1, d2, d3, d4, d5;
	Block *r;
	int i;
	char *p, *s = " stop";

	d2 = pop();
	for(p = s; *p; p++)
		sputc(d2.p, *p);
	d1 = pop();
	r = d1.b;
	for(i = 0; i < r->m; i++) {
		d3.val = r->ents[i]->vt;
		push(d3);
		d4.p = salloc(0);
		for(p = r->ents[i]->key; *p; p++)
			sputc(d4.p, *p);
		push(d4);
		d5.p = salloc(0);
		for(p = r->ents[i]->val; *p; p++)
			sputc(d5.p, *p);
		push(d5);
		pushio(d2.p);
		run();
	}
}
