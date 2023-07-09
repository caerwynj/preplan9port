/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include <threadassert.h>
#include "threadimpl.h"


ulong _threaddebuglevel;

void
_threaddebug(ulong l, char *fmt, ...)
{
	va_list arg;
	Thread *t;
	Proc *p;
	int n;
	char *buf;

	if ((l & _threaddebuglevel) == 0) return;
	t = getthread();
	p = t->proc;
	buf = zalloc(1024, 0);
	if (t->cmdname)
		n = sprint(buf, "%d.%d [%s] ", p->pid, t->id, t->cmdname);
	else
		n = sprint(buf, "%d.%d ", p->pid, t->id);

	va_start(arg, fmt);
	n = doprint(buf+n, buf+1024, fmt, arg) - buf;
	va_end(arg);

	buf[n++] = '\n';
	buf[n] = '\0';
	write(2, buf, n);
	free(buf);
}

void
_threadfatal(char *fmt, ...)
{
	va_list arg;
	Thread *t;
	Proc *p;
	int n;
	char *buf;

	t = getthread();
	p = t->proc;
	buf = zalloc(1024, 0);
	if (t->cmdname)
		n = sprint(buf, "*** %d.%d [%s] ", p->pid, t->id, t->cmdname);
	else
		n = sprint(buf, "*** %d.%d ", p->pid, t->id);

	va_start(arg, fmt);
	n = doprint(buf+n, buf+1024, fmt, arg) - buf;
	va_end(arg);

	buf[n++] = '\n';
	buf[n] = '\0';
	write(2, buf, n);
	free(buf);
	abort();
}

void
_threadassert(char *s)
{
	char *buf;
	Thread *t;
	Proc *p;
	int n;

	buf = zalloc(1024, 0);
	t = getthread();
	p = t->proc;
	n = sprint(buf, "%d.%d ", p->pid, t->id);
	write(2, buf, n);
	free(buf);
	abort();
}
