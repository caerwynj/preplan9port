/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include <thread.h>

void
threadmain(int argc, char *argv[])
{
	int nflag;
	int i, len;
	char *buf, *p;

	threadsetname("main");

	nflag = 0;
	if(argc > 1 && strcmp(argv[1], "-n") == 0)
		nflag = 1;

	len = 1;
	for(i = 1+nflag; i < argc; i++)
		len += strlen(argv[i])+1;

	buf = malloc(len);
	if(buf == 0)
		exits("no memory");

	p = buf;
	for(i = 1+nflag; i < argc; i++)
		p += sprint(p, i == argc-1 ? "%s":"%s ", argv[i]);
		
	if(!nflag)
		sprint(p, "\n");

	if(write(1, buf, strlen(buf)) < 0)
		fprint(2, "echo: write error: %r\n");

	exits((char *)0);
}
