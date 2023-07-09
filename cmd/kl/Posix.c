#include	"l.h"
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>	/* For sysconf() and _SC_CLK_TCK */

/*
 * fake malloc
 */
void*
malloc(ulong n)
{
	void *p;

	while(n & 7)
		n++;
	while(nhunk < n)
		gethunk();
	p = hunk;
	nhunk -= n;
	hunk += n;
	return p;
}

void
free(void *p)
{
	USED(p);
}

void*
calloc(ulong m, ulong n)
{
	void *p;

	n *= m;
	p = malloc(n);
	memset(p, 0, n);
	return p;
}

void*
realloc(void *p, ulong n)
{
	fprint(2, "realloc called\n", p, n);
	abort();
	return 0;
}

void*
mysbrk(ulong size)
{
	return (void*)sbrk(size);
}

double
cputime(void)
{

	struct tms tmbuf;
	double	ret_val;

	/*
	 * times() only fials if &tmbuf is invalid.
	 */
	(void)times(&tmbuf);
	/*
	 * Return the total time (in system clock ticks)
	 * spent in user code and system
	 * calls by both the calling process and its children.
	 */
	ret_val = (double)(tmbuf.tms_utime + tmbuf.tms_stime +
			tmbuf.tms_cutime + tmbuf.tms_cstime);
	/*
	 * Convert to seconds.
	 */
	ret_val *= sysconf(3);
	return ret_val;
	
}
