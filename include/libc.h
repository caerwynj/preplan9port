/* Plan 9 C library interface */

#define	nelem(x)	(sizeof(x)/sizeof((x)[0]))
/*#define	offsetof(s, m)	(ulong)(&(((s*)0)->m))*/
/*#define	assert(x)	if(x);else _assert("x")*/

/*
 * mem routines
 */
extern	void*	memccpy(void*, void*, int, ulong);
extern	void*	memset(void*, int, ulong);
extern	int	memcmp(void*, void*, ulong);
extern	void*	memcpy(void*, void*, ulong);
extern	void*	memmove(void*, void*, ulong);
extern	void*	memchr(void*, int, ulong);

/*
 * string routines
 */
extern	char*	strcat(char*, char*);
extern	char*	strchr(char*, int);
extern	int	strcmp(char*, char*);
extern	char*	strcpy(char*, char*);
extern	char*	strdup(char*);
extern	char*	strncat(char*, char*, long);
extern	char*	strncpy(char*, char*, long);
extern	int	strncmp(char*, char*, long);
extern	char*	strpbrk(char*, char*);
extern	char*	strrchr(char*, int);
extern	char*	strtok(char*, char*);
extern	long	strlen(char*);
extern	long	strspn(char*, char*);
extern	long	strcspn(char*, char*);
extern	char*	strstr(char*, char*);
extern	int	tokenize(char*, char**, int);


enum
{
	UTFmax		= 3,		/* maximum bytes per rune */
	Runesync	= 0x80,		/* cannot represent part of a utf sequence (<) */
	Runeself	= 0x80,		/* rune and utf sequences are the same (<) */
	Runeerror	= 0x80		/* decoding error in utf */
};

/*
 * rune routines
 */
extern	int	runetochar(char*, const Rune*);
extern	int	chartorune(Rune*, const char*);
extern	int	runelen(long);
extern	int	fullrune(char*, int);
extern	int	utflen(char*);
extern	int	utfnlen(char*, long);
extern	char*	utfrune(char*, long);
extern	char*	utfrrune(char*, long);
extern	char*	utfutf(char*, char*);
extern	Rune	tolowerrune(Rune);
extern	Rune	totitlerune(Rune);
extern	Rune	toupperrune(Rune);
extern	int	isalpharune(Rune);
extern	int	islowerrune(Rune);
extern	int	isspacerune(Rune);
extern	int	istitlerune(Rune);
extern	int	isupperrune(Rune);

/*
 * malloc
 */
extern	void*	malloc(ulong);
extern	void*	calloc(ulong, ulong);
extern	void*	realloc(void*, ulong);
extern	void	free(void*);
/*
extern	void*	mallocz(ulong, int);
extern	ulong	msize(void*);
extern	void		setmalloctag(void*, ulong);
extern	void		setrealloctag(void*, ulong);
extern	ulong	getmalloctag(void*);
extern	ulong	getrealloctag(void*);
extern	void*	malloctopoolblock(void*);
*/

/*
 * print routines
 */
typedef	struct	Fconv	Fconv;
struct	Fconv
{
	char*	out;		/* pointer to next output */
	char*	eout;		/* pointer to end */
	int	f1;
	int	f2;
	int	f3;
	int	chr;
};
extern	char*	doprint(char *s, char *es, const char *fmt, va_list argp);
extern	int	print(const char *fmt, ...);
extern	int	fprint(int f, char *fmt, ...);
extern	int	sprint(char *buf, char *fmt, ...);
extern	int	snprint(char *buf, int len, char *fmt, ...);
extern	char*	seprint(char *buf, char *e, char *fmt, ...);

extern	double	charstod(int (*f)(void *), void *a);
extern	int	fmtinstall(int, int (*)(va_list*, Fconv*));
extern	int	numbconv(va_list*, Fconv*);
extern	void	strconv(char*, Fconv*);
extern	void	Strconv(Rune*, Fconv*);
extern	int	fltconv(va_list*, Fconv*);

/*
 * random number
 */
extern	void	srand(long);
extern	int	rand(void);
extern	int	nrand(int);
extern	long	lrand(void);
extern	long	lnrand(long);
extern	double	frand(void);
extern	ulong	truerand(void);			/* uses /dev/random */
extern	int	n_truerand(int);		/* uses /dev/random */
extern	ulong	fastrand(void);			/* uses /dev/random extended by a prng */

/*
 * math
 */
extern	double	NaN(void);
extern	int	isNaN(double d);
extern	double	Inf(int sign);
extern	int	isInf(double d, int sign);

extern	double	pow(double, double);
extern	double	atan2(double, double);
extern	double	fabs(double);
extern	double	atan(double);
extern	double	log(double);
extern	double	log10(double);
extern	double	exp(double);
extern	double	floor(double);
extern	double	ceil(double);
extern	double	hypot(double, double);
extern	double	sin(double);
extern	double	cos(double);
extern	double	tan(double);
extern	double	asin(double);
extern	double	acos(double);
extern	double	sinh(double);
extern	double	cosh(double);
extern	double	tanh(double);
extern	double	sqrt(double);
extern	double	fmod(double, double);

#define	HUGE	3.4028234e38
#define	PIO2	1.570796326794896619231e0
#define	PI	(PIO2+PIO2)

/*
 * Time-of-day
 */

typedef
struct Tm
{
	int	sec;
	int	min;
	int	hour;
	int	mday;
	int	mon;
	int	year;
	int	wday;
	int	yday;
	char	zone[4];
	int	tzoff;
} Tm;

extern	Tm*	gmtime(long);
extern	Tm*	localtime(long);
extern	char*	asctime(Tm*);
extern	char*	ctime(long);


extern	void	_assert(char*);
extern	int	abs(int);
extern	int	atexit(void(*)(void));
extern	void	atexitdont(void(*)(void));
extern	int	atnotify(int(*)(void*, char*), int);
extern	double	atof(char*);
extern	int	atoi(char*);
extern	long	atol(char*);
extern	vlong	atoll(char*);
extern	double	charstod(int(*)(void*), void*);
extern	char*	cleanname(char*);
extern	int	decrypt(void*, void*, int);
extern	int	encrypt(void*, void*, int);
extern	void	exits(char*);
extern	double	frexp(double, int*);
extern	ulong	getcallerpc(void*);
extern	char*	getenv(char*);
extern	int	getfields(char*, char**, int, int, char*);
extern	char*	getuser(void);
extern	char*	getwd(char*, int);
extern	long	labs(long);
extern	double	ldexp(double, int);
/*extern	void	longjmp(jmp_buf, int); */
extern	char*	mktemp(char*);
extern	double	modf(double, double*);
extern	int	netcrypt(void*, void*);
/*extern	void	notejmp(void*, jmp_buf, int);*/
extern  int	postnote(int, int, char *);
#define pow10(n) (pow(10.0, (double)(n)))
/*extern	int	putenv(char*, char*);*/
extern	void	qsort(void*, long, long, int (*)(void*, void*));
extern	char*	setfields(char*);
/*extern	int	setjmp(jmp_buf);*/
extern	double	strtod(char*, char**);
extern	long	strtol(char*, char**, int);
extern	ulong	strtoul(char*, char**, int);
extern	vlong	strtoll(char*, char**, int);
extern	uvlong	strtoull(char*, char**, int);
extern	void	sysfatal(char*, ...);
extern	void	syslog(int, char*, char*, ...);
extern	long	time(long*);
extern	int	tolower(int);
extern	int	toupper(int);

/* 
 * Synchronization
 * Solaris locks 
 */
#include <sys/synch.h>
typedef lwp_mutex_t mutex_t;
typedef mutex_t Lock;
extern int	mutex_lock(mutex_t *);
extern int	mutex_trylock(mutex_t *);
extern int	mutex_unlock(mutex_t *);

#define lock(x) 	mutex_lock(x)
#define unlock(x) 	mutex_unlock(x)
extern int canlock(Lock *);

typedef Lock QLock;
#define qlock(x) 	mutex_lock(x)
#define qunlock(x) 	mutex_unlock(x)
#define canqlock(x) 	canlock(x)


/*
 * system calls
 *
 */
#define	NAMELEN	28	/* length of path name element */
#define	ERRLEN	64	/* length of error string */
#define	DIRLEN	116	/* length of machine-independent Dir structure */
#define	OREAD	0	/* open for read */
#define	OWRITE	1	/* write */
#define	ORDWR	2	/* read and write */
#define	OEXEC	3	/* execute, == read but check execute permission */
#define	OTRUNC	16	/* or'ed in (except for exec), truncate file first */
#define	OCEXEC	32	/* or'ed in, close on exec */
#define	ORCLOSE	64	/* or'ed in, remove on close */
#define	OEXCL	0x1000	/* or'ed in, exclusive use */
#define AEXEC	0x1
#define AWRITE	0x2
#define AREAD	0x4
#define ERRLEN	64
#define NAMELEN	28

#define	NCONT	0	/* continue after note */
#define	NDFLT	1	/* terminate after note */
#define	NSAVE	2	/* clear note but hold state */
#define	NRSTR	3	/* restore saved state */

#define CHDIR		0x80000000	/* mode bit for directories */
#define CHAPPEND	0x40000000	/* mode bit for append only files */
#define CHEXCL		0x20000000	/* mode bit for exclusive use files */
#define CHMOUNT		0x10000000	/* mode bit for mounted channel */
#define CHREAD		0x4		/* mode bit for read permission */
#define CHWRITE		0x2		/* mode bit for write permission */
#define CHEXEC		0x1		/* mode bit for execute permission */

#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt (rubout) */
#define	SIGQUIT	3	/* quit (ASCII FS) */
#define	SIGILL	4	/* illegal instruction (not reset when caught) */
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#define	SIGIOT	6	/* IOT instruction */
#define	SIGABRT 6	/* used by abort, replace SIGIOT in the future */
#define	SIGEMT	7	/* EMT instruction */
#define	SIGFPE	8	/* floating point exception */
#define	SIGKILL	9	/* kill (cannot be caught or ignored) */
#define	SIGBUS	10	/* bus error */
#define	SIGSEGV	11	/* segmentation violation */
#define	SIGSYS	12	/* bad argument to system call */
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#define	SIGUSR1	16	/* user defined signal 1 */
#define	SIGUSR2	17	/* user defined signal 2 */
#define	SIGCLD	18	/* child status change */
#define	SIGCHLD	18	/* child status change alias (POSIX) */
#define	SIGPWR	19	/* power-fail restart */
#define	SIGWINCH 20	/* window size change */
#define	SIGURG	21	/* urgent socket condition */
#define	SIGPOLL 22	/* pollable event occured */
#define	SIGIO	SIGPOLL	/* socket I/O possible (SIGPOLL alias) */
#define	SIGSTOP 23	/* stop (cannot be caught or ignored) */
#define	SIGTSTP 24	/* user stop requested from tty */
#define	SIGCONT 25	/* stopped process has been continued */
#define	SIGTTIN 26	/* background tty read attempted */
#define	SIGTTOU 27	/* background tty write attempted */
#define	SIGVTALRM 28	/* virtual timer expired */
#define	SIGPROF 29	/* profiling timer expired */
#define	SIGXCPU 30	/* exceeded cpu limit */
#define	SIGXFSZ 31	/* exceeded file size limit */
#define	SIGWAITING 32	/* process's lwps are blocked */
#define	SIGLWP	33	/* special signal used by thread library */
#define	SIGFREEZE 34	/* special signal used by CPR */
#define	SIGTHAW 35	/* special signal used by CPR */
#define	SIGCANCEL 36	/* thread cancellation signal used by libthread */
#define	SIGLOST	37	/* resource lost (eg, record-lock lost) */

typedef	struct	Qid	Qid;
typedef	struct	Dir	Dir;

struct	Qid
{
	ulong	path;
	ulong	vers;
};

struct Dir
{
	char	name[NAMELEN];
	char	uid[NAMELEN];
	char	gid[NAMELEN];
	Qid	qid;
	ulong	mode;
	long	atime;
	long	mtime;
	Length	len;
	short	type;
	short	dev;
};

extern	int	dirstat(char*, Dir*);
extern	int	dirfstat(int, Dir*);
extern	int	dirwstat(char*, Dir*);
extern	int	dirfwstat(int, Dir*);


/*#define wait(x) __plan9wait(x)*/
#define nbwait(x) __plan9nbwait(x)

typedef
struct Waitmsg {
	char	pid[12];         /* of loved one */
	char	time[3*12];      /* of loved one & descendants */
	char	msg[ERRLEN];
} Waitmsg;			/* Plan 9 wait */
/*extern	int	wait(Waitmsg *w);*/	/* Plan 9 wait */
extern	int	nbwait(Waitmsg *w);	/* Plan 9 wait */


extern	void	abort(void);
#define		create(name, mode, perm)	creat(name, perm)
extern	int	chdir(char*);
extern	int	close(int);
#define		dup(a,b)			dup2(a,b)
extern	int	errstr(char *);
#define		exec(a,b)			execv(a,b)
extern	int	notify(void(*)(void *, char *));
extern	int	open(char*, int);
extern	long	read(int, void*, long);
extern	void*	sbrk(ulong);
#define		seek(a,b,c)			lseek(a,b,c)
extern	int	sleep(long);
extern	long	write(int, void*, long);

#define	USED(a)
#define SET(a)
#define nil ((void*)0)

#define	_exits(v)			if (v!=0) _exit(1); else _exit(0)

#define	ARGBEGIN	for((argv0? 0: (argv0= *argv)),argv++,argc--;\
			    argv[0] && argv[0][0]=='-' && argv[0][1];\
			    argc--, argv++) {\
				char *_args, *_argt, _argc;\
				_args = &argv[0][1];\
				if(_args[0]=='-' && _args[1]==0){\
					argc--; argv++; break;\
				}\
				_argc=0;while(*_args) switch(_argc= *_args++)
#define	ARGEND		}
#define	ARGF()		(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	EARGF(x)	(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): ((x), exit(1), (char*)0)))

#define	ARGC()		_argc
extern char *argv0;

