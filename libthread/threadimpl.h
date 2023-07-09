/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include "thread.h"

int setpgid(int pid, int pgid);

typedef enum {
	None,
	Running,
	Runnable,
	Rendezvous,
	Exiting,
} State;
	
typedef enum {
	Callnil,
	Callalt,
	Callsnd,
	Callrcv,
} Callstate;

enum {
	DOEXEC = 1,
	DOEXIT = 2,
	DOPROC = 3,
};

struct Tqueue {		/* Thread queue */
	Lock	lock;
	Thread	*head;
	Thread	*tail;
};

struct Thread {
	Lock		lock;		/* protects thread data structure */
	int		id;		/* thread id */
	int 		grp;		/* thread group */
	State		state;		/* state of thread */
	Callstate	call;		/* which `system call' is current */
	char		*cmdname;	/* ptr to name of thread */
	Thread		*next;		/* next on queue (run, rendezvous) */
	Thread		*nextt;		/* next on list of all theads */
	Proc		*proc;		/* proc of this thread */
	ulong		tag;		/* rendez-vous tag */
	Alt		*alt;		/* pointer to alt structure */
	ulong		value;		/* rendez-vous value */
	jmp_buf		env;		/* jump buf for launching or switching threads */
};

struct Proc {
	Lock	lock;
	int	pid;

	int	nthreads;
	Tqueue	threads;		/* All threads of this proc */
	Tqueue	runnable;		/* Runnable threads */
	Thread	*curthread;		/* Running thread */

	char	msg[ERRLEN];		/* exit status */

	int	blocked;		/* In a rendezvous */
	int	exec;			/* Has exec-ed */
	Proc	*parent;
	Proc	*child;			/* linked list of children (via sibling) */
	Proc	*sibling;		/* next sibling */
	Ref	mustwait;		/* incremented by dying children */
	ulong	udata;			/* User per-proc data pointer */
};

typedef struct Newproc {
	uchar	*stack;
	uint	stacksize;
	ulong	*stackptr;
	ulong	launcher;
	int		grp;
} Newproc;

typedef struct Execproc {
	char *file;
	void **arg;
} Execproc;

struct Pqueue {		/* Proc queue */
	Lock	lock;
	Proc	*head;
	Proc	*tail;
};

#define MAXTHREAD 1024
Lock		threadlock;
struct Threadtab {
	void	*stacklo;
	void	*stackhi;
	Thread	*thread;
};

extern struct Threadtab	threadtab[MAXTHREAD];	/* Protected by threadlock */

extern Ref nprocs;
extern struct Pqueue pq;	/* Linked list of procs */
extern Lock threadlock;

int	tas(int*);
int	inc(int*, int);
int	cas(Lock *lock, Lock old, Lock new);
ulong	_threadrendezvous(Thread *t, ulong, ulong);
void	_threadexits(Thread *t, char *status);
void	_yield(Thread *t);

void	_xinc(long *);
long	_xdec(long *);

/* Linux */
#define setthread(t)
