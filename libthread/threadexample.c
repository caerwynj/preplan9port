/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include <thread.h>
#include <fatal.h>

#define STACKSIZE (2*1024)

void
addthread(void *ac) {
	Channel* addchan = mc;

	if ((mfd = open("/dev/mouse", OREAD)) < 0)
		error("open /dev/mouse: %r\n");
	for (;;) {
		if (read(mfd, &m, sizeof(m)) != sizeof(m) ||
		  atoi(m+25) & 4) { /* EOF || button 3 down */
			error("quit\n");
		}
		send(mousechan, &m);
	}
}

void
clockproc(void *cc) {
	int t = 0;
	Channel* clockchan = cc;

	while (++t) {
		sleep(1000);
		send(clockchan, &t);
	}
}


void
threadmain(int argc, char *argv[]) {
	char m[48];
	int t;
	Alt a[] = {
	/*	 c	v	op   */
		{nil,	&m,	CHANRCV},
		{nil,	&t,	CHANRCV},
		{nil,	nil,	CHANEND},
	};

	/* create mouse event channel and mouse process */
	a[0].c = chancreate(sizeof(m), 0);
	proccreate(mouseproc, (void *)(a[0].c), STACKSIZE);

	/* create clock event channel and clock process */
	a[1].c = chancreate(sizeof(t), 0);	/* clock event channel */
	proccreate(clockproc, (void *)(a[1].c), STACKSIZE);

	for (;;) {
		switch(alt(a)) {
		case 0:	/*mouse event */
			fprint(2, "click ");
			break;
		case 1:	/* clock event */
			fprint(2, "tic ");
			break;
		default:
			error("impossible");
		}
	}
}
