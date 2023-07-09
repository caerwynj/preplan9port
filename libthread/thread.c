/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include <sys/stat.h>
#include <threadassert.h>
#include "threadimpl.h"

static void	garbagethread(Thread *t);
static Thread*	getqbytag(Proc *p, Tqueue *, ulong);
static void	putq(Tqueue *, Thread *);
static Thread*	getq(Tqueue *);
static void	threadprep(Thread *t, void *stacklo, void *stackhi);

typedef struct Proclaunch {
	Thread *t;
	void (*f)(void *arg);
	void *arg;
} Proclaunch;

static Channel*	thewaitchan;
static int	pgrp;

static Ref	nprocs;		/* number of procs */
static Tqueue	rendez;
static Lock	rendezlock;

Lock		threadlock;
struct Threadtab	threadtab[MAXTHREAD];	/* Protected by threadlock */
static long	nextid;		/* Protected by threadlock */

Channel *
threadwaitchan(void) {
	thewaitchan = chancreate(sizeof(Waitmsg), 50);
	_threaddebug(DBGWAIT, "thewaitchan = 0x%lux", thewaitchan);
	return thewaitchan;
}

static void
badhandler(int s) {
	_threaddebug(DBGPROC, "unexpected signal: %d", s);
	abort();
}

static void
deadchildhandler(int s) {
	_threaddebug(DBGWAIT, "Dead children");
}

static void
deadhandler(int s) {
	Thread *t;
	Proc *p;
	/* Pass signal on to siblings and children */
	t = getthread();
	p = t->proc;
	_threaddebug(DBGPROC, "passing on signal %d to 0x%lux and 0x%lux",
		s, p->child, p->sibling);
	if (p->child)
		kill(p->child->pid, s);
	if (p->sibling)
		kill(p->sibling->pid, s);
	_exits(0);
}

static void
chandler(int s) {
	_threaddebug(DBGREND, "cont signal seen");
}

ulong
threadrendezvous(ulong tag, ulong value) {
	return _threadrendezvous(getthread(), tag, value);
}

void
threadinit(void) {
	Thread *t;
	Proc *p;

	p = zalloc(sizeof *p, 1);
	t = zalloc(sizeof *t, 1);
	t->proc = p;
	qlockinit(threadrendezvous);
	pgrp = p->pid = getpid();
	setthread(t);
	signal(SIGCHLD, deadchildhandler);
	signal(SIGCONT, chandler);
	signal(SIGINT, deadhandler);
	threadprep(t, (void *)((ulong)&t - 0x1000), (void *)0xffffffff);
	incref(&nprocs);
	t->state = Running;
	getq(&p->runnable);
}

static void
threadprep(Thread *t, void *stacklo, void *stackhi) {
	int i;
	Proc *p = t->proc;

	lock(&threadlock);
	t->id = nextid++;
	i = 0;
	for (;;) {
		if (threadtab[i].thread == 0) {
			threadtab[i].stacklo = stacklo;
			threadtab[i].stackhi = stackhi;
			threadtab[i].thread = t;
			break;
		}
		if (++i == MAXTHREAD)
			_threadfatal("Can't find stack");
	}
	unlock(&threadlock);
	lock(&p->lock);
	p->nthreads++;
	if (p->threads.head == nil) {
		p->threads.head = p->threads.tail = t;
	} else {
		p->threads.tail->nextt = t;
		p->threads.tail = t;
	}
	t->next = (Thread *)~0;
	putq(&p->runnable, t);
	unlock(&p->lock);
}

static void
garbagethread(Thread *t) {
	Proc *p;
	int i;
	Thread *r, *pr;

	threadassert(t->state == Exiting);
	p = t->proc;
	pr = nil;
	for (r = p->threads.head; r; r = r->nextt) {
		if (r == t)
			break;
		pr = r;
	}
	threadassert (r != nil);
	if (pr)
		pr->nextt = r->nextt;
	else
		p->threads.head = r->nextt;
	if (p->threads.tail == r)
		p->threads.tail = pr;
	if (t->cmdname)
		free(t->cmdname);
	lock(&threadlock);
	for (i = 0; i < MAXTHREAD; i++) {
		if (threadtab[i].thread == t)
			break;
	}
	if (i == MAXTHREAD)
		_threadfatal("Can't find thread");
	if (i == 0)
		return;
	free(threadtab[i].stacklo);
	threadtab[i].stacklo = nil;
	threadtab[i].stackhi = nil;
	threadtab[i].thread = nil;
	unlock(&threadlock);
	free(t);
}

static void
threadlauncher(void (*f)(void *arg), void *arg, Thread *t) {
	threadassert(t == getthread());
	_threaddebug(DBGTHRD, "Threadlauncher calling %lux(%lux)", f, arg);
	(*f)(arg);
	_threaddebug(DBGTHRD, "threadlauncher: %lux(%lux) returned", f, arg);
	_threadexits(t, nil);
}

static int
proclauncher(void *arg) {
	Proclaunch *l = arg;
	Thread *t = l->t;

	threadassert(t == getthread());
	getq(&t->proc->runnable);
/*	if (setpgid(0, pgrp) < 0) */
/*		_threadfatal("setpgid: %r"); */
	
	signal(SIGCONT, chandler);
	signal(SIGINT, deadhandler);
	signal(SIGSEGV, badhandler);
	_threaddebug(DBGPROC, "Proclauncher calling %lux(%lux)", l->f, l->arg);
	(*l->f)(l->arg);
	_threaddebug(DBGPROC, "Proclauncher: %lux(%lux) returned", l->f, l->arg);
	free(l);
	_threadexits(t, nil);
	return 0;
}

int
proccreate(void (*f)(void *arg), void *arg, uint stacksize) {
	Thread *t, *parentthread;
	Proc *parent;
	Proc *p;
	ulong *stack;
	Proclaunch *l;
	int pid;

	parentthread = getthread();
	parent = parentthread->proc;
	t =	mallocz(sizeof *t, 1);
	p =	mallocz(sizeof *p, 1);
	stack =	mallocz(stacksize, 0);
	l =	mallocz(sizeof(*l), 0);
	if (l == nil || p == nil || stack == nil || t == nil) {
		if (l) free(l);
		if (p) free(p);
		if (t) free(t);
		if (stack) free(stack);
		errno = ENOMEM;
		return -1;
	}
	stacksize /= sizeof(ulong);

	t->grp = parentthread->grp;
	t->state = Running;
	t->proc = p;
	incref(&nprocs);
	p->parent = parent;
	p->sibling = parent->child;
	parent->child = p;
	threadprep(t, (void*)stack, (void*)(stack + stacksize));
	l->t = t;
	l->f = f;
	l->arg = arg;
	if((pid = __clone(proclauncher, stack+stacksize, CLONE_VM|SIGCHLD, l)) <= 0)
		_threadfatal("Can't clone: %r");
	p->pid = pid;
	_threaddebug(DBGPROC, "New proc %d (%lux), created by %d (%lux)",
		pid, p, parent->pid, parent);
	return pid;
}

int
threadcreate(void (*f)(void *arg), void *arg, uint stacksize) {
	Thread *t, *parent;
	Proc *p;
	ulong *stack;

	parent = getthread();
	t =	mallocz(sizeof *t, 1);
	stack =	mallocz(stacksize, 0);

	if (t == nil || stack == nil) {
		if (t) free(t);
		if (stack) free(stack);
		errno = ENOMEM;
		return -1;
	}

	stacksize /= sizeof(ulong);

	p = parent->proc;
	t->grp = parent->grp;
	t->state = Runnable;
	t->proc = p;
	threadprep(t, (void*)stack, (void*)(stack + stacksize));
	/* one of these will hold arg: */
	stack[stacksize-1] = (ulong)t;
	stack[stacksize-2] = (ulong)arg;
	stack[stacksize-3] = (ulong)f;
	t->env->__jmpbuf[JB_SP] = (int)(stack + stacksize - 4);
	t->env->__jmpbuf[JB_PC] = (int)threadlauncher;
	_threaddebug(DBGTHRD, "New thread %d (%lux), f=%lux, arg=%lux, created by %d (%lux)",
		t->id, t, f, arg, parent->id, parent);
	return t->id;
}

static void
deadchild(Proc *p, int pid, Waitmsg *w) {
	Proc *childp;
	Thread *childt;
	Channel *c;

	childp = p->child;
	threadassert(childp != nil);
	_threaddebug(DBGWAIT, "Waited for 0x%lux(%d)",
		childp, childp->pid);
	if (childp->pid == pid) {
		p->child = childp->sibling;
	} else {
		Proc *pc = childp;
		while ((childp = childp->sibling)) {
			if (childp->pid == pid) {
				pc->sibling = childp->sibling;
				break;
			}
			pc = childp;
		}
		threadassert(childp != nil);
	}
	childt = childp->threads.head;
	/* Need exactly one child: */
	threadassert(childt != nil && childt->nextt == nil);
	_threaddebug(DBGWAIT, "Cleaning up thread 0x%lux", childt);
	garbagethread(childt);
	memcpy(w->msg, childp->msg, sizeof(childp->msg));
	free(childp);
	if ((c = thewaitchan) != nil) {	/* store exposed global in local variable c */
		_threaddebug(DBGWAIT, "Sending wait messge");
		send(c, w);
	}
	decref(&p->mustwait);
}

void
_threadexits(Thread *t, char *status) {
	Proc *p;

	p = t->proc;
	t->state = Exiting;
	_threaddebug(DBGTHRD, "Threadexits");
	if (--p->nthreads == 0) {
		/* last thread in proc */
		if (decref(&nprocs) == 0)
			exits(status);
		_threaddebug(DBGPROC, "Last thread not in last proc");
		if (status && status[0])
			strncpy(p->msg, status, ERRLEN);
		while (p->child) {
			Waitmsg w;
			int pid;

			/* wait for children */
			_threaddebug(DBGWAIT, "Delaying exit of %d for child",
				p->pid);
			pid = wait(&w);
			if (pid < 0) {
				_threaddebug(DBGWAIT, "Expected another dead child");
				break;
			}
			deadchild(p, pid, &w);
		}
		/* Make parent clean up */
		if (p->parent)	/* Don't clean up for main */
			incref(&p->parent->mustwait);
		exits(status);
	}
	_yield(t);
}

void
threadexits(char *status) {
	_threadexits(getthread(), status);
}

void
_yield(Thread *t) {
	Thread *new;
	Proc *p;
	ulong thr;
	int pid;
	Waitmsg w;

	p = t->proc;

	if (p->mustwait.ref && (pid = nbwait(&w)) > 0)
		deadchild(p, pid, &w);
	lock(&rendezlock);
	if ((new = getq(&p->runnable)) == nil) {
		int pid;
		Waitmsg w;

		threadassert(new != t);
		_threaddebug(DBGTHRD, "Nothing to yield for");
		if (t->state == Exiting) {
			_threaddebug(DBGPROC, "Wait for rendezvous");
			p->blocked = 1;
			while ((new = getq(&p->runnable)) == nil) {
				unlock(&rendezlock);
				usleep(10000);
				if (p->mustwait.ref && (pid = nbwait(&w)) > 0)
					deadchild(p, pid, &w);
				lock(&rendezlock);
			}
			p->blocked = 0;
			unlock(&rendezlock);
			new->state = Running;
			longjmp(new->env, (int)t);
			/* no return */
		}
		unlock(&rendezlock);
		return;	/* Nothing to yield for */
	}
	unlock(&rendezlock);
	if (t->state == Running || t->state == Runnable) {
		if ((thr = setjmp(t->env))) {
			if (thr != ~0)
				garbagethread((Thread *)thr);
			return;	/* Return from yielding */
		}
		putq(&p->runnable, t);
		t->state = Runnable;
		t = (Thread *)~0;
	}
	_threaddebug(DBGTHRD, "Yielding to %d.%d", p->pid, new->id);
	new->state = Running;
	setthread(new);
	longjmp(new->env, (long)t);
	/* no return */
}

void
yield(void) {
	_yield(getthread());
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
getqbytag(Proc *p, Tqueue *q, ulong tag) {
	Thread *r, *pr, *w, *pw;
	
	w = pr = pw = nil;
	lock(&q->lock);
	for (r = q->head; r; r = r->next) {
		if (r->tag == tag) {
			w = r;
			pw = pr;
			if (r->proc == p) {
				/* Locals or blocked remotes are best */
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

ulong
_threadrendezvous(Thread *this, ulong tag, ulong value) {
	Proc *p;
	Thread *that, *new;
	ulong v, t;
	int pid;
	Waitmsg w;

	p = this->proc;

	lock(&rendezlock);
	_threaddebug(DBGREND, "rendezvous tag %lux", tag);
	/* find a thread waiting in a rendezvous on tag */
	that = getqbytag(p, &rendez, tag);
	/* if a thread in same or another proc waiting, rendezvous */
	if (that) {
		_threaddebug(DBGREND, "waiting proc is %lud.%d", that->proc->pid, that->id);
		threadassert(that->state == Rendezvous);
		/* exchange values */
		v = that->value;
		that->value = value;
		/* remove from rendez-vous queue */
		that->state = Runnable;
		_threaddebug(DBGREND, "put %lud.%d on runq", that->proc->pid, that->id);
		putq(&that->proc->runnable, that);
		unlock(&rendezlock);
		if (that->proc->blocked && that->proc != p)
			kill(that->proc->pid, SIGCONT);
		return v;
	}
	_threaddebug(DBGREND, "blocking");
	/* Mark this thread waiting */
	this->value = value;
	this->state = Rendezvous;
	this->tag = tag;
	putq(&rendez, this);

	/* Look for runnable threads */
	while ((new = getq(&p->runnable)) == nil) {
		/* No other thread runnable, rendezvous */
		_threaddebug(DBGREND, "blocking rendezvous, tag = %lux", (ulong)p);
		p->blocked = 1;
		unlock(&rendezlock);
		usleep(10000);
		if (p->mustwait.ref && (pid = nbwait(&w)) > 0)
			deadchild(p, pid, &w);
		lock(&rendezlock);
	}
	p->blocked = 0;
	unlock(&rendezlock);
	if ((t = setjmp(this->env))) {
		if (t != ~0)
			garbagethread((Thread *)t);
		_threaddebug(DBGREND, "unblocking");
		threadassert(this->next == (Thread *)~0);
		threadassert(this->state == Running);
		return this->value;
	}
	_threaddebug(DBGREND|DBGTHRD, "Scheduling %lud.%d", new->proc->pid, new->id);
	new->state = Running;
	setthread(new);
	longjmp(new->env, ~0);
	/* no return */
	return ~0;	/* Not called */
}

void
threadsetname(char *name)
{
	Thread *t = getthread();

	if (t->cmdname)
		free(t->cmdname);
	t->cmdname = strdup(name);
}

char *threadgetname(void) {
	return getthread()->cmdname;
}

int
threadprint(int fd, char *fmt, ...)
{
	int n;
	va_list arg;
	char *buf;

	buf = zalloc(1024, 0);
	if(buf == nil)
		return -1;
	va_start(arg, fmt);
	doprint(buf, buf+1024, fmt, arg);
	va_end(arg);
	n = write(fd, buf, strlen(buf));
	free(buf);
	return n;
}

void
abort(void) {
	while (1) {
		write(2, "aborted\n", 8);
		sleep(10);
	}
}

void
threadexitsall(char *status) {
	Thread *t;

	_threaddebug(DBGPROC, "threadexitsall");
	t = getthread();
	if (t->proc->pid != pgrp) {
		_threaddebug(DBGPROC, "killing main");
		kill(pgrp, SIGINT);
	} else {
		_threaddebug(DBGPROC, "killing wife and children");
		deadhandler(SIGINT);
	}
	exits(status);
}

void
procexecl(char *f, ...) {

	procexec(f, &f+1);
}

void
procexec(char *f, char *args[]) {
	Thread *t;
	Proc *p;
	struct stat sb;

	/* make sure exec is likely to succeed before tossing thread state */
	if(stat(f, &sb) < 0 || S_ISREG(sb.st_mode) == 0 || access(f, AEXEC) < 0)
		return;
	t = getthread();
	p = t->proc;
	if(p->threads.head != t || p->threads.head->nextt != nil)
		return;

	/* Make sure proc state is cleaned up after exec succeeds */
	decref(&nprocs);
	_threaddebug(DBGPROC, "Preparing exec");
	/* Make parent clean up */
	if (p->parent)
		incref(&p->parent->mustwait);
	t->state = Exiting;
	execv(f, args);
	exits("exec");
	/* No return; */
}
