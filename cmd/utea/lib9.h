#define const	

#include <utf.h>
#include <fmt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <bio.h>
#include <time.h>

#define	CHDIR	0x80000000	/* mode bit for directories */
#define	NAMELEN	28	/* length of path name element */
#define	ERRLEN	64	/* length of error string */
#define	DIRLEN	116	/* length of machine-independent Dir structure */
#define	OREAD	O_RDONLY
#define	OWRITE	O_WRONLY
#define	ORDWR	O_RDWR
#define	USED(x)	if(x);else
#define	remove	unlink
#define	seek	lseek
#define	exits(s)	exit((s) && ((char*)s)[0] ? 1 : 0)
#define	create(name, mode, perm)	creat(name, perm)
#define	nil	((void*)0)

#define uchar _9uchar
#define ushort _9ushort
#define uint _9uint
#define ulong _9ulong
#define vlong _9vlong
#define uvlong _9uvlong

typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

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
	ulong	length;
	short	type;
	short	dev;
};

extern	int	dirstat(char*, Dir*);
extern	int	dirfstat(int, Dir*);
extern	int	dirwstat(char*, Dir*);
extern	int	dirfwstat(int, Dir*);

#define	ARGBEGIN	for((argv0? (char*)0: (argv0= *argv)),argv++,argc--;\
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
