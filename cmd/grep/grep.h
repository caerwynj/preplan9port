#include	<u.h>
#include	<libc.h>
#include	<bio.h>

#ifndef	EXTERN
#define	EXTERN	extern
#endif

typedef	struct	Re	Re;
typedef	struct	Re2	Re2;
typedef	struct	State	State;

struct	State
{
	int	count;
	Re**	re;
	State*	linkleft;
	State*	linkright;
	State**	next;
	uchar	match;
};
struct	Re2
{
	Re*	beg;
	Re*	end;
};
struct	Re
{
	uchar	type;
	ushort	gen;
	union 
	{
		Re*	alt;	/* Talt */
		Re**	cases;	/* case */
		struct		/* class */
		{
			Rune	lo;
			Rune	hi;
		}_c;	
		Rune	val;	/* char */
	}_u;
	Re*	next;
};

enum
{
	Talt		= 1,
	Tbegin,
	Tcase,
	Tclass,
	Tend,
	Tor,

	Caselim		= 7,
	Nhunk		= 1<<16,
	Cbegin		= 0x10000,
	Flshcnt		= (1<<9)-1,

	Cflag		= 1<<0,
	Hflag		= 1<<1,
	Iflag		= 1<<2,
	Llflag		= 1<<3,
	LLflag		= 1<<4,
	Nflag		= 1<<5,
	Sflag		= 1<<6,
	Vflag		= 1<<7,
	Bflag		= 1<<8
};

EXTERN	union
{
	char	string[16*1024];
	struct 
	{
		uchar	pre[1*1024];	/* to save to previous '\n' */
		uchar	buf[16*1024];	/* input buffer */
	}_s;
} u;

EXTERN	char	*filename;
EXTERN	Biobuf	bout;
EXTERN	char	flags[256];
EXTERN	Re**	follow;
EXTERN	ushort	gen;
EXTERN	char*	input;
EXTERN	long	lineno;
EXTERN	int	literal;
EXTERN	int	matched;
EXTERN	long	maxfollow;
EXTERN	long	nfollow;
EXTERN	int	peekc;
EXTERN	Biobuf*	rein;
EXTERN	State*	state0;
EXTERN	Re2	topre;

extern	Re*	addcase(Re*);
extern	void	appendnext(Re*, Re*);
extern	void	error(char*);
extern	int	fcmp(void*, void*); 	/* (Re**, Re**) */
extern	void	fol1(Re*, int);
extern	int	getrec(void);
extern	void	increment(State*, int);
extern	State*	initstate(Re*);
extern	void*	mal(int);
extern	void	patchnext(Re*, Re*);
extern	Re*	ral(int);
extern	Re2	re2cat(Re2, Re2);
extern	Re2	re2class(char*);
extern	Re2	re2or(Re2, Re2);
extern	Re2	re2char(int, int);
extern	Re2	re2star(Re2);
extern	State*	sal(int);
extern	int	search(char*, int);
extern	void	str2top(char*);
extern	int	yyparse(void);
extern	void	reprint(char*, Re*);
extern	void	yyerror(char*, ...);
