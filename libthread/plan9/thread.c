#include <u.h>
#include <lib9.h>
#include "assert.h"
#include "threadimpl.h"

static void	initproc(void (*)(ulong, int, char*[]), int, char*[], uint);
static void	garbagethread(Thread *t);
static Thread*	getqbytag(Tqueue *, ulong);
static void	putq(Tqueue *, Thread *);
static Thread*	getq(Tqueue *);
static void	waitranday(Proc *);
static void	launcher(ulong, void (*)(void *), void *);
static void	mainlauncher(ulong, int argc, char *argv[]);
static void	garbageproc(Proc *);
static Proc*	prepproc(Newproc *);

static int	notefd;
static int	passerpid;
static long	totalmalloc;

static Thread*	dontkill;

extern int __isthreaded;

int		threadrforkflag;
int		threadhack;
Channel*	thewaitchan;

struct Pqueue	pq;
Proc		**procp;	// Pointer to pointer to proc's Proc structure
Tqueue		rendez;
Lock		rendezlock;

#define STACKOFF 2
#ifdef M386
	#define FIX1
	#define FIX2 *--tos = 0
#endif
#ifdef Mmips
	#define FIX1
	#define FIX2
#endif
#ifdef Mpower
	/* This could be wrong ... */
	#define FIX1
	#define FIX2 *--tos = 0
#endif
#ifdef Malpha
	#define FIX1 *--tos = 0
	#define FIX2 *--tos = 0
#endif

static int
notehandler(void *, char *s) {
	Proc *p;
	Thread *t;
	int id;

	if (DBGNOTE & _threaddebuglevel)
		fprint(2, "Got note %s\n", s);
	if (getpid() == passerpid) {
		if (DBGNOTE & _threaddebuglevel)
			fprint(2, "Notepasser %d got note %s\n", passerpid, s);
		write(notefd, s, strlen(s));
		if (strncmp(s, "kilthr", 6) == 0 || strncmp(s, "kilgrp", 6) == 0)
			return 1;
	} else {
		if (strncmp(s, "kilthr", 6) == 0) {
			p = *procp;
			id = strtoul(s+6, nil, 10);
			if (DBGNOTE & _threaddebuglevel)
				fprint(2, "Thread id %d", id);
			for (t = p->threads.head; t; t = t->nextt) {
				if (t->id == id) {
					break;
				}
			}
			if (t == nil) {
				_threaddebug(DBGNOTE, "Thread id %d not found", id);
				return 1;
			}
			threadassert(t != p->curthread || t->state != Running);
			threadassert(t->state != Rendezvous);
			t->exiting = 1;
			return 1;
		}
		if (strncmp(s, "kilgrp", 6) == 0) {
			p = *procp;
			id = strtoul(s+6, nil, 10);
			_threaddebug(DBGNOTE, "Thread grp %d", id);
			for (t = p->threads.head; t; t = t->nextt) {
				if (t->grp == id) {
					if (t != p->curthread)
						t->exiting = 1;
				}
			}
			return 1;
		}
	}
	if (strncmp(s, "kilall", 6) == 0)
		exits(s+6);
	return 0;
}

void
threadexits(char *exitstr) {
	Thread *t, *x;
	Proc *p;
	Channel *c;

	_threaddebug(DBGTHRD|DBGKILL, "Exitthread()");
	p = *procp;
	t = p->curthread;
	assert (t->state == Running);
	if (p->nthreads <= 1) {
		// Clean up and exit
		if (exitstr)
			p->arg = strdup(exitstr);
		else
			p->arg = nil;
		/* thewaitchan confounds exiting the entire program, so handle it carefully */
		if ((c = thewaitchan) != nil) {	/* store exposed global in local variable c */
			Waitmsg w;
			long t[4];

			snprint(w.pid, sizeof(w.pid), "%d", p->pid);
			times(t);
			snprint(w.time + 0*12, 12, "%10.2fu", t[0]/1000.0);
			snprint(w.time + 1*12, 12, "%10.2fu", t[1]/1000.0);
			snprint(w.time + 2*12, 12, "%10.2fu",
				(t[0]+t[1]+t[2]+t[3])/1000.0);	// Cheating just a bit
			if (exitstr)
				strncpy(w.msg, exitstr, sizeof(w.msg));
			else
				w.msg[0] = '\0';
			_threaddebug(DBGCHLD, "In thread %s: sending exit status %s for %d\n",
				p->curthread->cmdname, exitstr, p->pid);
			send(c, &w);
		}
		_threaddebug(DBGPROC, "Exiting\n");
		t->exiting = 1;
		p->nthreads--;
		longjmp(p->oldenv, DOEXIT);
	}
	t->exiting = 1;
	p->nthreads--;
	lock(&rendezlock);
	if ((x = getq(&p->runnable)) == nil) {
		_threaddebug(DBGTHRD, "Nothing left to run");
		waitranday(p);
		// No return
	}
	unlock(&rendezlock);
	_threaddebug(DBGTHRD, "Yield atexit to %d.%d", p->pid, x->id);
	p->curthread = x;
	x->state = Running;
	if (x->exiting)
		threadexits(nil);
	longjmp(x->env, (int)t);
	// no return
}

void
threadkillgrp(int grp) {
	int fd;
	char buf[128];

	sprint(buf, "/proc/%d/notepg", getpid());
	fd = open(buf, OWRITE|OCEXEC);
	sprint(buf, "kilgrp%d", grp);
	write(fd, buf, strlen(buf));
	close(fd);
}

void
threadkill(Thread *t) {
	int fd;
	char buf[128];

	sprint(buf, "/proc/%d/notepg", getpid());
	fd = open(buf, OWRITE|OCEXEC);
	sprint(buf, "kilthr%d", t->id);
	write(fd, buf, strlen(buf));
	close(fd);
}

void
threadexitsall(char *status) {
	int fd;
	Channel *c;
	char buf[128];

	if ((c = thewaitchan) != nil) {
		thewaitchan = nil;
		while (nbrecv(c, nil))
			;	/* Unblock threads previously exited,
				 * sending on thewaitchan, not yet waited for
				 */
	}
	sprint(buf, "/proc/%d/note", passerpid);
	fd = open(buf, OWRITE);
	if(status == nil)
		status = "";
	snprint(buf, sizeof buf, "kilall%s", status);
	write(fd, buf, strlen(buf));
	exits(status);
}

void
yield(void) {
	Thread *new, *t;
	Proc *p;
	ulong thr;

	p = *procp;
	t = p->curthread;

	if (t->exiting) {
		_threaddebug(DBGTHRD|DBGKILL, "Exiting in yield()");
		threadexits(nil);	/* no return */
	}
	if ((new = getq(&p->runnable)) == nil) {
		_threaddebug(DBGTHRD, "Nothing to yield for");
		return;	/* Nothing to yield for */
	}
	if ((thr = setjmp(p->curthread->env))) {
		if (thr != ~0)
			garbagethread((Thread *)thr);
		if ((*procp)->curthread->exiting)
			threadexits(nil);
		return;	/* Return from yielding */
	}
	putq(&p->runnable, p->curthread);
	p->curthread->state = Runnable;
	_threaddebug(DBGTHRD, "Yielding to %d.%d", p->pid, new->id);
	p->curthread = new;
	new->state = Running;
	longjmp(new->env, ~0);
	/* no return */
}

ulong
threadrendezvous(ulong tag, ulong value) {
	Proc *p;
	Thread *this, *that, *new;
	ulong v, t;

	p = *procp;
	this = p->curthread;

	lock(&rendezlock);
	_threaddebug(DBGREND, "rendezvous tag %lux", tag);
	/* find a thread waiting in a rendezvous on tag */
	that = getqbytag(&rendez, tag);
	/* if a thread in same or another proc waiting, rendezvous */
	if (that) {
		_threaddebug(DBGREND, "waiting proc is %lud.%d", that->proc->pid, that->id);
		threadassert(that->state == Rendezvous);
		/* exchange values */
		v = that->value;
		that->value = value;
		/* remove from rendez-vous queue */
		that->state = Runnable;
		if (that->proc->blocked) {
			threadassert(that->proc != p);
			that->proc->blocked = 0;
			_threaddebug(DBGREND, "unblocking rendezvous, tag = %lux", (ulong)that->proc);
			unlock(&rendezlock);
			/* `Non-blocking' rendez-vous */
			while (rendezvous((ulong)that->proc, (ulong)that) == ~0) {
				_threaddebug(DBGNOTE|DBGTHRD, "interrupted");
				if (this->exiting) {
					_threaddebug(DBGNOTE|DBGTHRD, "and committing suicide");
					threadexits(nil);
				}
			}
		} else {
			putq(&that->proc->runnable, that);
			unlock(&rendezlock);
		}
		yield();
		return v;
	}
	_threaddebug(DBGREND, "blocking");
	/* Mark this thread waiting */
	this->value = value;
	this->state = Rendezvous;
	this->tag = tag;
	putq(&rendez, this);

	/* Look for runnable threads */
	new = getq(&p->runnable);
	if (new == nil) {
		/* No other thread runnable, rendezvous */
		p->blocked = 1;
		_threaddebug(DBGREND, "blocking rendezvous, tag = %lux", (ulong)p);
		unlock(&rendezlock);
		while ((new = (Thread *)rendezvous((ulong)p, 0)) == (Thread *)~0) {
			_threaddebug(DBGNOTE|DBGTHRD, "interrupted");
			if (this->exiting) {
				_threaddebug(DBGNOTE|DBGTHRD, "and committing suicide");
				threadexits(nil);
			}
		}
		threadassert(!p->blocked);
		threadassert(new->proc == p);
		if (new == this) {
			this->state = Running;
			if (this->exiting)
				threadexits(nil);
			return this->value;
		}
		_threaddebug(DBGREND, "after rendezvous, new thread is %lux, id %d", new, new->id);
	} else {
		unlock(&rendezlock);
	}
	if ((t = setjmp(p->curthread->env))) {
		if (t != ~0)
			garbagethread((Thread *)t);
		_threaddebug(DBGREND, "unblocking");
		threadassert(p->curthread->next == (Thread *)~0);
		this->state = Running;
		if (p->curthread->exiting) {
			_threaddebug(DBGKILL, "Exiting after rendezvous");
			threadexits(nil);
		}
		return this->value;
	}
	_threaddebug(DBGREND|DBGTHRD, "Scheduling %lud.%d", new->proc->pid, new->id);
	p->curthread = new;
	new->state = Running;
	longjmp(new->env, ~0);
	/* no return */
	return ~0;	/* Not called */
}

void
main(int argc, char *argv[]) {
	int pid;
	Waitmsg w;
	char buf[128], err[ERRLEN];

	__isthreaded = 1;
//	_threaddebuglevel = DBGNOTE;
	if(threadrforkflag){
		pid = rfork(threadrforkflag);
		if(pid < 0){
			fprint(2, "libthread: initial rfork failed: %r\n");
			exits("rfork");
		}
		if(pid > 0){	/* result is pid from RFPROC; parent should exit */
			while(threadhack)
				sleep(100);
			exits(nil);
		}
	}
	qlockinit(threadrendezvous);
	atnotify(notehandler, 1);
	pid = rfork(RFPROC|RFMEM|RFREND|RFNOTEG);
	threadassert(pid >= 0);
	if (pid) {
		rfork(RFCFDG);
		passerpid = getpid();
		sprint(buf, "/proc/%d/notepg", pid);
		notefd = open(buf, OWRITE|OCEXEC);
		/* wait for notes and pass them on */
		for (;;)
			if(wait(&w) < 0) {
				errstr(err);
				if (strcmp(err, "interrupted") != 0)
					break;
			}
		exits(w.msg);
		/* never returns */
	} else {
		threadassert(pid == 0);
		initproc(mainlauncher, argc, argv, 256*1024);
	}
}

void
threadsetname(char *name)
{
	Thread *t = (*procp)->curthread;

	if (t->cmdname)
		free(t->cmdname);
	t->cmdname = strdup(name);
}

char *threadgetname(void) {
	return (*procp)->curthread->cmdname;
}

ulong *
procdata(void) {
	return &((*procp)->udata);
}

int
proccreate(void (*f)(void *), void *arg, uint stacksize) {
	Newproc *np;
	Proc *p, *pp;
	ulong *tos;

	p = *procp;
	/* Save old stack position */
	if ((pp = (Proc *)setjmp(p->curthread->env))) {
		_threaddebug(DBGPROC, "newproc, return\n");
		return pp->pid;	/* Return with pid of new proc */
	}
	np = _threadmalloc(sizeof(Newproc));
	threadassert(np != nil);

	_threaddebug(DBGPROC, "newproc, creating stack\n");
	/* Create a stack */
	np->stack = _threadmalloc(stacksize);
	threadassert(np->stack != nil);
	memset(np->stack, 0xFE, stacksize);
	tos = (ulong *)(&np->stack[stacksize&(~7)]);
	FIX1;
	*--tos = (ulong)arg;
	*--tos = (ulong)f;
	FIX2;
	np->stacksize = stacksize;
	np->stackptr = tos;
	np->grp = p->curthread->grp;
	np->launcher = (long)launcher;

	_threaddebug(DBGPROC, "newproc, switch stacks\n");
	/* Goto unshared stack and fire up new process */
	p->arg = np;
	longjmp(p->oldenv, DOPROC);
	/* no return */
	return -1;
}

int
threadcreate(void (*f)(void *arg), void *arg, uint stacksize) {
	Thread *child;
	ulong *tos;
	Proc *p;

	p = *procp;
	if (stacksize < 32) {
		werrstr("%s", "stacksize");
		return -1;
	}
	if ((child = _threadmalloc(sizeof(Thread))) == nil ||
		(child->stk = _threadmalloc(child->stksize = stacksize)) == nil) {
			if (child) free(child);
			werrstr("%s", "_threadmalloc");
			return -1;
	}
	memset(child->stk, 0xFE, stacksize);
	p->nthreads++;
	child->cmdname = nil;
	child->id = ++p->nextID;
	child->proc = p;
	tos = (ulong *)(&child->stk[stacksize&(~7)]);
	FIX1;
	*--tos = (ulong)arg;
	*--tos = (ulong)f;
	FIX2;	/* Insert a dummy argument on 386 */
	child->env[JMPBUFPC] = ((ulong)launcher+JMPBUFDPC);
	/* -STACKOFF leaves room for old pc and new pc in frame */
	child->env[JMPBUFSP] = (ulong)(tos - STACKOFF);
	child->state = Runnable;
	child->exiting = 0;
	child->nextt = nil;
	if (p->threads.head == nil) {
		p->threads.head = p->threads.tail = child;
	} else {
		p->threads.tail->nextt = child;
		p->threads.tail = child;
	}
	child->next = (Thread *)~0;
	putq(&p->runnable, child);
	return child->id;
}

void
procexecl(Channel *pidc, char *f, ...) {

	procexec(pidc, f, &f+1);
}

void
procexec(Channel *pidc, char *f, char *args[]) {
	Proc *p;
	Dir d;
	int n, pid;
	Execproc *ep;
	Channel *c;

	/* make sure exec is likely to succeed before tossing thread state */
	if(dirstat(f, &d) < 0 || (d.mode & CHDIR) || access(f, AEXEC) < 0) {
bad:	if (pidc) sendul(pidc, ~0);
		return;
	}
	p = *procp;
	if(p->threads.head != p->curthread || p->threads.head->nextt != nil)
		goto bad;

	n = 0;
	while (args[n++])
		;
	ep = _threadmalloc(sizeof(Execproc));
	threadassert(ep != nil);
	ep->file = strdup(f);
	ep->arg = _threadmalloc(n*sizeof(char *));
	if (ep->arg == nil || ep->file == nil) goto bad;
	ep->arg[--n] = nil;
	while (--n >= 0)
		ep->arg[n] = strdup(args[n]);

	/* committed to exec-ing */
	if ((pid = setjmp(p->curthread->env))) {
		int wpid, i;
		Waitmsg w;

		rfork(RFFDG);
		close(0);
		close(1);
		for (i = 3; i < 100; i++)
			close(i);
		if(pidc != nil)
			send(pidc, &pid);
		/* wait for exec-ed child */
		while ((wpid = wait(&w)) != pid)
			threadassert(wpid != -1);
		if ((c = thewaitchan) != nil) {	/* global is exposed */
			_threaddebug(DBGCHLD, "Sending exit status for exec: real pid %d, fake pid %d, status %s\n", pid, getpid(), w.msg);
			send(c, &w);
		}
		_threaddebug(DBGPROC, "Exiting (exec)\n");
		threadexits("libthread procexec");
	}
	p->arg = ep;
	longjmp(p->oldenv, DOEXEC);
	/* No return; */
}
H
int
threadgetgrp(void) {
	return (*procp)->curthread->grp;
}

int
threadsetgrp(int ng) {
	int og;

	og = (*procp)->curthread->grp;
	(*procp)->curthread->grp = ng;
	return og;
}

void *
_threadmalloc(long size) {
	void *m;

	m = malloc(size);
	if (m == nil) {
		fprint(2, "Malloc of size %ld failed: %r\n", size);
		abort();
	}
	setmalloctag(m, getcallerpc(&size));
	totalmalloc += size;
	if (size > 1000000) {
		fprint(2, "Malloc of size %ld, total %ld\n", size, totalmalloc);
		abort();
	}
	return m;
}

static void
runproc(Proc *p) {
	int action, pid;
	Proc *pp;
	long r;

	r = ~0;
runp:
	// Leave a proc manager
	while ((action = setjmp(p->oldenv))) {
		Newproc *np;
		Execproc *ne;
		char *s;

		p = *procp;
		switch(action) {
		case DOEXEC:
			ne = (Execproc *)p->arg;
			if ((pid = rfork(RFPROC|RFREND|RFNOTEG|RFFDG)) < 0) {
				exits("doexecproc: fork: %r");
			}
			if (pid == 0) {
				exec(ne->file, ne->arg);
				exits("Can't exec");
				// No return
			}
			longjmp(p->curthread->env, pid);
			// No return
		case DOEXIT:
			_threaddebug(DBGPROC, "at doexit\n");
			s = (char *)p->arg;
			lock(&pq.lock);
			if (pq.head == p) {
				pq.head = p->next;
				if (pq.tail == p)
					pq.tail = nil;
			} else {
				for (pp = pq.head; pp->next; pp = pp->next) {
					if(pp->next == p) {
						pp->next = p->next;
						if (pq.tail == p)
							pq.tail = pp;
						break;
					}
				}
			}
			unlock(&pq.lock);
			garbageproc(p);
			exits(s);
		case DOPROC:
			_threaddebug(DBGPROC, "at doproc\n");
			np = (Newproc *)p->arg;
			pp = prepproc(np);
			free(np);
			if ((pid = rfork(RFPROC|RFMEM)) < 0) {
				exits("donewproc: fork: %r");
			}
			if (pid == 0) {
				// Child is the new proc; touch old proc struct no more
				p = pp;
				p->pid = getpid();
				goto runp;
				// No return
			}
			// Parent, return to caller
			r = (long) pp;
			_threaddebug(DBGPROC, "newproc, unswitch stacks\n");
			break;
		default:
			// `Can't happen'
			threadassert(0);
		}
	}

	// Jump into proc
	*procp = p;
	longjmp(p->curthread->env, r);
	// No return
}

static void
initproc(void (*f)(ulong, int argc, char *argv[]), int argc, char *argv[], uint stacksize) {
	Proc *p;
	Newproc *np;
	ulong *tos;
	ulong *av;
	int i;

	procp = (Proc **)argv;	// any address on a private stack

	// Create a stack and fill it
	np = _threadmalloc(sizeof(Newproc));
	threadassert(np != nil);
	np->stack = _threadmalloc(stacksize);
	threadassert(np->stack != nil);
	memset(np->stack, 0xFE, stacksize);
	tos = (ulong *)(&np->stack[stacksize&(~7)]);
	np->stacksize = stacksize;
	np->grp = 0;
	for (i = 0; i < argc; i++){
		char *nargv;
	
		nargv = (char *)tos - (strlen(argv[i]) + 1);
		strcpy(nargv, argv[i]);
		argv[i] = nargv;
		tos = (ulong *)nargv;
	}
	// round down to address of char*
	tos = (ulong *)((ulong)tos & ~0x3);
	tos -= argc + 1;
	// round down to address of vlong (for the alpha):
	tos = (ulong *)((ulong)tos & ~0x7);
	av = tos;
	memmove(av, argv, (argc+1)*sizeof(char *));
	FIX1;
	*--tos = (ulong)av;
	*--tos = (ulong)argc;
	FIX2;
	np->stackptr = tos;
	np->launcher = (long)f;
	p = prepproc(np);
	p->pid = getpid();
	free(np);
	runproc(p);
	// no return;
}

static void
garbageproc(Proc *p) {
	Thread *t, *nextt;

	for (t = p->threads.head; t; t = nextt) {
		if (t->cmdname)
			free(t->cmdname);
		threadassert(t->stk != nil);
		free(t->stk);
		nextt = t->nextt;
		free(t);
	}
	free(p);
}

static void
garbagethread(Thread *t) {
	Proc *p;
	Thread *r, *pr;

	p = t->proc;
	pr = nil;
	for (r = p->threads.head; r; r = r->nextt) {
		if (r == t)
			break;
		pr = r;
	}
	assert (r != nil);
	if (pr)
		pr->nextt = r->nextt;
	else
		p->threads.head = r->nextt;
	if (p->threads.tail == r)
		p->threads.tail = pr;
	if (t->cmdname)
		free(t->cmdname);
	threadassert(t->stk != nil);
	free(t->stk);
	free(t);
}

static void
putq(Tqueue *q, Thread *t) {
	lock(&q->lock);
	_threaddebug(DBGQUE, "Putq 0x%lux on 0x%lux, next == 0x%lux",
		(ulong)t, (ulong)q, (ulong)t->next);
	threadassert((ulong)(t->next) == (ulong)~0);
	t->next = nil;
	if (q->head == nil) {
		q->head = q->tail = t;
	} else {
		threadassert(q->tail->next == nil);
		q->tail->next = t;
		q->tail = t;
	}
	unlock(&q->lock);
}

static Thread *
getq(Tqueue *q) {
	Thread *t;

	lock(&q->lock);
	if ((t = q->head)) {
		q->head = q->head->next;
		t->next = (Thread *)~0;
	}
	unlock(&q->lock);
	_threaddebug(DBGQUE, "Getq 0x%lux from 0x%lux", (ulong)t, (ulong)q);
	return t;
}

static Thread *
getqbytag(Tqueue *q, ulong tag) {
	Thread *r, *pr, *w, *pw;

	w = pr = pw = nil;
	lock(&q->lock);
	for (r = q->head; r; r = r->next) {
		if (r->tag == tag) {
			w = r;
			pw = pr;
			if (r->proc == *procp) {
				// Locals or blocked remotes are best
				break;
			}
		}
		pr = r;
	}
	if (w) {
		if (pw)
			pw->next = w->next;
		else
			q->head = w->next;
		if (q->tail == w)
			q->tail = pw;
		w->next = (Thread *)~0;
	}
	unlock(&q->lock);
	_threaddebug(DBGQUE, "Getqbytag 0x%lux from 0x%lux", w, q);
	return w;
}

static void
waitranday(Proc *p) {
	Thread *new, *t;

	// called with rendezlock held
	p->blocked = 1;
	unlock(&rendezlock);
	t = p->curthread;
	while ((new = (Thread *)rendezvous((ulong)p, 0)) == (Thread *)~0) {
		_threaddebug(DBGNOTE|DBGTHRD, "interrupted");
		if (t->exiting) {
			_threaddebug(DBGNOTE|DBGTHRD, "and committing suicide");
			threadexits(nil);
		}
	}
	threadassert(!p->blocked);
	threadassert(new->proc == p);
	p->curthread = new;
	new->state = Running;
	longjmp(new->env, (int)t);
	// no return
}

static void
launcher(ulong, void (*f)(void *arg), void *arg) {

	(*f)(arg);
	threadexits(nil);
	// no return
}

static void
mainlauncher(ulong, int argc, char *argv[]) {
//	ulong *p;

//	p = (ulong *)&argc;
//	fprint(2, "p[-2..2]: %lux %lux %lux %lux %lux\n",
//		p[-2], p[-1], p[0], p[1], p[2]);

	threadmain(argc, argv);
	threadexits(nil);
	// no return
}

static Proc *
prepproc(Newproc *np) {
	Proc *p;
	Thread *t;

	// Create proc and thread structs
	p = _threadmalloc(sizeof(Proc));
	t = _threadmalloc(sizeof(Thread));
	if (p == nil || t == nil) {
		char err[ERRLEN] = "";
		errstr(err);
		write(2, err, strlen(err));
		write(2, "\n", 1);
		exits("procinit: _threadmalloc");
	}
	memset(p, 0, sizeof(Proc));
	memset(t, 0, sizeof(Thread));
	t->cmdname = strdup("threadmain");
	t->id = ++p->nextID;
	t->grp = np->grp;	// Inherit grp id
	t->proc = p;
	t->state = Running;
	t->nextt = nil;
	t->next = (Thread *)~0;
	t->stk = np->stack;
	t->stksize = np->stacksize;
	t->env[JMPBUFPC] = (np->launcher+JMPBUFDPC);
	// -STACKOFF leaves room for old pc and new pc in frame
	t->env[JMPBUFSP] = (ulong)(np->stackptr - STACKOFF);
//fprint(2, "SP = %lux\n", t->env[JMPBUFSP]);
	p->curthread = t;
	p->threads.head = p->threads.tail = t;
	p->nthreads = 1;
	p->pid = 0;
	p->next = nil;

	lock(&pq.lock);
	if (pq.head == nil) {
		pq.head = pq.tail = p;
	} else {
		threadassert(pq.tail->next == nil);
		pq.tail->next = p;
		pq.tail = p;
	}
	unlock(&pq.lock);

	return p;
}

int
threadprint(int fd, char *fmt, ...)
{
	int n;
	va_list arg;
	char *buf;

	buf = _threadmalloc(1024);
	if(buf == nil)
		return -1;
	va_start(arg, fmt);
	doprint(buf, buf+1024, fmt, arg);
	va_end(arg);
	n = write(fd, buf, strlen(buf));
	free(buf);
	return n;
}
