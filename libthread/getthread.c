/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include "threadimpl.h"

Thread *
getthread(void) {
	Thread *t;
	int i;

	t = nil;
	lock(&threadlock);
	for (i = 0; i < MAXTHREAD; i++) {
		if (threadtab[i].thread == 0)
			continue;
		if (threadtab[i].stacklo < (void *)&i
		 && (void *)&i < threadtab[i].stackhi) {
		 	t = threadtab[i].thread;
			break;
		}
	}
	unlock(&threadlock);
	if (t == nil) {
		threadprint(2, "*** Can't find thread\n");
		abort();
	}
	return t;
}

int
procpid(void) {
	return getthread()->proc->pid;
}
