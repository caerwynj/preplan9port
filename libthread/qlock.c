/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#include "lock.h"
#include "qlock.h"

struct Qlock {
	Lock	l;	/* spinlock protecting this data structure */
	int	locked;	/* lock itself */
	int	nw;	/* number of waiters */
	pid_t	*w;	/* malloc-ed, list of waiters */
};

void
qlock(Qlock *l) {
	lock(&l->l);
	while (l->locked) {
		l->w = realloc(l->w, (l->nw+1)*sizeof(pid_t));
		if (l->w == NULL) fatal("qlock: malloc");
		l->w[l->nw] = getpid();
		l->nw++;
		unlock(&l->l);
		pause();
		lock(&l->l);
	}
	l->locked++;
	unlock(&l->l);
}

void
qunlock(Qlock *l) {
	int n;
	pid_t p = 0;

	lock(&l->l);
	l->locked == 0;
	if (l->nw) {
		n = (int) ((float)l->nw*rand()/(RAND_MAX+1.0));
		p = l->w[n];
		memmove(l->w + n, l->w + n + 1, l->nw - n - 1);
		l->nw--;
	}
	unlock(&l->l);
	if (p) kill(p, SIGCONT);
}
