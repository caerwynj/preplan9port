/* Copyright (c) 1998 Lucent Technologies - All rights reserved. */
/* #include <stdlib.h>
 * #include <string.h>
 * #include <sys/types.h>
 * #include <stdio.h>
 * #include <unistd.h>
 */
#define _STRING_H
#define _STDLIB_H
#define _STDIO_H
#define _UNISTD_H
#define _ISO_TIME_ISO_H
#define __TIME
#define _STDARG_H
#include <setjmp.h>
typedef unsigned char	uchar;
typedef	unsigned short	Rune;
typedef	unsigned short	Ushort;
typedef	unsigned char	Uchar;
typedef unsigned long	Ulong;
typedef struct{
		Ulong	length;
		Ulong	hlength;
}Length;
	/* System configuration parameters */

#ifdef	SYSVR3
#include	<malloc.h>
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#define	remove(v)			unlink(v)
#define	WEXITSTATUS(s)			(((s)>>8)&0xFF)
extern	char *getenv(char*);
extern	char *getlogin(void);
extern	char *strerror(int);
extern	void *memmove(void*, const void*, size_t);
#define	NEEDMEMMOVE
#define	NEEDSTRERROR
#define	NEEDVARARG
#endif	/* SYSVR3 */

#ifdef	IRIX5
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#endif	/* IRIX5 */

#ifdef	IRIX
extern	void *memmove(void*, const void*, size_t);
#define	NEEDMEMMOVE
#endif	/* IRIX */

#ifdef	UMIPS
typedef unsigned long	ulong;
typedef	unsigned short	ushort;
#define	const			/* mips compiler doesn't support const */
extern	char *strerror(int);
extern	void *memmove(void*, const void*, size_t);
#define	NEEDMEMMOVE
#define	NEEDSTRERROR
#define	NEEDVARARG
#define	NOFIFO			/* turn off exstart in samterm/unix.c */
#endif	/* UMIPS */

#ifdef	SUNOS
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
extern	char *strerror(int);
extern	void *memmove(void*, const void*, size_t);
extern	void *memcpy(void*, const void*, size_t);
#define	NEEDMEMMOVE
#define	NEEDSTRERROR
#endif	/* SUNOS */

#ifdef	SOLARIS
extern	char *strerror(int);
typedef signed char	schar;
typedef unsigned int 	uint;
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
typedef unsigned long long	vlong;
typedef unsigned long long	uvlong;

#define _VA_LIST
typedef char *__va_list;
typedef __va_list va_list;

#define va_start(list, start) ((void)((list) = (sizeof(start)<4 ? \
	(char *)((int *)&(start)+1) : (char *)(&(start)+1))))
#define __va_arg(list, mode, n) \
	(*(mode *)(&(list += ((sizeof(mode)+n)&~n))[-(int)((sizeof(mode)+n)&~n)]))
#define _bigendian_va_arg(list, mode, n) (\
	sizeof(mode)==1 ? *(mode *)(&(list += 4)[-1]) : \
	sizeof(mode)==2 ? *(mode *)(&(list += 4)[-2]) : __va_arg(list, mode, n))
#define _littleendian_va_arg(list, mode, n) __va_arg(list, mode, n)
#define va_end(list) ((void)0)
#define va_arg(list, mode) (sizeof(mode)>8 ? \
	**(mode **)(&(list += 4)[-4]) : \
	_bigendian_va_arg(list, mode, 3U))
#endif

#ifdef	AIX
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#endif	/* AIX */

#ifdef	OSF1
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
extern	void *memmove(void*, const void*, size_t);
#endif	/* OSF1 */

#ifdef  HPUX
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#define	NEEDSTRERROR
#endif  /* HPUX */

#ifdef  APOLLO
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#endif  /* APOLLO */

#ifdef  CONVEX
typedef unsigned long	ulong;
#endif  /* CONVEX */

#ifdef  DYNIX
#define	SIG_ERR		BADSIG
#define	NEEDMEMMOVE
#define	remove(v)			unlink(v)
#define	WEXITSTATUS(s)			(((s)>>8)&0xFF)
#define	NEEDMEMMOVE
#define	NOFIFO			/* turn off exstart in samterm/unix.c */
#endif  /* DYNIX */

#ifdef	PTX
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#endif	/* PTX */

#ifdef	BSDi
typedef unsigned long   ulong;
#endif	/* BSDi */

#ifdef	v10
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
#endif
