#include <u.h>
#include <libc.h>
#include <threadassert.h>
#include <fatal.h>
#include "threadimpl.h"

#define nprocs 1
#define nthreads 3
#define nchans 6
#define alts 1
#define s_r 1

#define STACKSIZE (512*1024)

Channel *gc[nchans];
char *progname;

void
altthread(void *xid) {
	long id = (long)xid;
	Channel *cfrom = gc[id%nchans];
	Channel *cto = gc[(id+1)%nchans];
	ulong vin, vout;
	Alt a[3] = {
		{cfrom,	(uchar *)&vin,	CHANRCV},
		{cto,	(uchar *)&vout,	CHANSND},
		{nil,	nil,		CHANEND}
	};

	vin = 0;
	vout = 100*(id+1);
	_threaddebug(DBGAPPL, "altthread starting %lux/%lux, from %d to %d", &xid, xid, id%nchans, (id+1)%nchans);

	while (1) {
		_threaddebug(DBGAPPL, "alt receive %d send %d", id%nchans, (id+1)%nchans);
		switch (alt(a)) {
		case 0:	
			print("Thread %lud received %lud on %d\n", id, vin, id%nchans);
			break;
		case 1:
			print("Thread %lud sent %lud on %d\n", id, vout, (id+1)%nchans);
			break;
		default:
			fatal("Alt error %r in thread %d", id);
		}
	}
	abort();
}

void
s_rthread(void *xid) {
	Channel *cfrom, *cto;
	ulong v;
	int id = (long)xid;

	cfrom = gc[id%nchans];
	cto = gc[(id+1)%nchans];
	_threaddebug(DBGAPPL, "s_rthread starting %lux, from %d to %d", &xid, id%nchans, (id+1)%nchans);
	v = (id+1)*100;

	while (1) {
		_threaddebug(DBGAPPL, "send on %d", (id+1)%nchans);
		if (send(cto, (void *)&v) != 1) {
			fatal("Send error %r in thread %d", id);
		}
		print("Thread %lud sent %lud\n", id, v);
		_threaddebug(DBGAPPL, "receive on %d", id%nchans);
		if (recv(cfrom, &v) != 1) {
			fatal("Receive error %r in thread %d", id);
		}
		print("Thread %lud received %lud\n", id, v);
	}
	abort();
}

void
testproc(void *xid) {
	int i;
	Proc *p;
	int id = (long)xid;

	p = getthread()->proc;

	for (i = 1; i < nthreads; i++) {
		if (s_r)
			threadcreate(s_rthread, (void *)(id*nthreads + i), STACKSIZE);
		if (alts)
			threadcreate(altthread, (void *)(nthreads*nprocs + id*nthreads + i), STACKSIZE);
	}
	switch(alts*2 + s_r) {
	case 0:
		fatal("Must have alts or s_r");
	case 1:
		s_rthread((void *)(nthreads*nprocs + id*nthreads));
		break;
	case 3:
		threadcreate(s_rthread, (void *)(id*nthreads), STACKSIZE);
		/* fall through */
	case 2:
		altthread((void *)(nthreads*nprocs + id*nthreads));
		break;
	}
	abort();
}

void
main(int argc, char *argv[]) {
	int i;
	Waitmsg w;
	progname = argv[0];

//	_threaddebuglevel = DBGREND|DBGAPPL|DBGREND;
	threadinit();
	/* Create channels, make odd channels buffered */
	for (i = 0; i < nchans; i++) {
		if ((gc[i] = chancreate(sizeof(ulong), (i&1)?0:1)) == nil)
			fatal("Newchan returned nil");
	}
	if (nprocs == 1) {
		testproc(0);
#ifdef undef
	} else {
		for (i = 0; i < nprocs; i++) {
			if (proccreate(testproc, (void **)i, STACKSIZE) == nil) {
				print("Can't start proc: %r\n");
				exits("procerror");
			}
		}
		while (wait(&w) > 0)
			;
#endif
	}
}
