#include <u.h>
#include <libc.h>
#include <bio.h>
#include <ctype.h>
#include "btree.h"

#define	debug	0


#define BLK sizeof(Blk)
#define PTRSZ sizeof(int*)
#define HEADSZ 1024
#define RDSKSZ 100
#define length(p)	((p)->wt-(p)->beg)
#define rewind(p)	(p)->rd=(p)->beg
#define screate(p)	(p)->rd = (p)->wt = (p)->beg
#define fsfile(p)	(p)->rd = (p)->wt
#define truncate(p)	(p)->wt = (p)->rd
#define sfeof(p)	(((p)->rd==(p)->wt)?1:0)
#define sfbeg(p)	(((p)->rd==(p)->beg)?1:0)
#define sungetc(p)	--(p)->rd
#define sgetc(p)	(((p)->rd==(p)->wt)?-1:*(p)->rd++)
#define skipc(p)	{if((p)->rd<(p)->wt)(p)->rd++;}
#define slookc(p)	(((p)->rd==(p)->wt)?-1:*(p)->rd)
#define sbackc(p)	(((p)->rd==(p)->beg)?-1:*(--(p)->rd))
#define backc(p)	{if((p)->rd>(p)->beg) --(p)->rd;}
#define sputc(p,c)	{if((p)->wt==(p)->last)more(p);\
				*(p)->wt++ = c; }
#define salterc(p,c)	{if((p)->rd==(p)->last)more(p);\
				*(p)->rd++ = c;\
				if((p)->rd>(p)->wt)(p)->wt=(p)->rd;}
#define sunputc(p)	(*((p)->rd = --(p)->wt))
#define sclobber(p)	((p)->rd = --(p)->wt)
#define zero(p)		for(pp=(p)->beg;pp<(p)->last;)\
				*pp++='\0'


typedef struct Symbol	Symbol;
typedef struct Datum 	Datum;
typedef struct Saveval	Saveval;
typedef union Symval	Symval;
typedef struct	Blk	Blk;

struct	Blk
{
	char	*rd;
	char	*wt;
	char	*beg;
	char	*last;
};


union Symval
{ 
	double	val;
	void	*ptr;
};

struct Symbol
{
	char	*name;
	long	type;
	Symval u;
	struct Symbol	*next;
};
Symbol	*install(char*, int, double), *slookup(char*);

struct Datum
{
	int type;
	union {
		double	val;
		Blk	*p;		/* Dynamic string */
		Block *b;		/* Result set */
		char *s;		/* Fixed string */
		Symbol	*sym;
	};
};
extern Symbol*	sym;


Datum pop(void);
double Fgetd(int);
int	moreinput(void);
void	add(void);
void	and(void);
void	assign(void);
void	bltin(void);
void	btopen(void);
void	constpush(void);
void	div(void);
void	eq(void);
void	eval(void);
void	execerror(char*, char*);
void	forcode(void);
void	ge(void);
void	gt(void);
void	ifcode(void);
void	initcode(void);
void	le(void);
void	lt(void);
void	mod(void);
void	mul(void);
void	ne(void);
void	negate(void);
void	not(void);
void	now(void);
void	or(void);
void	load(void);
void	power(void);
void	prexpr(void);
void	printtop(void);
void	push(Datum);
void	strpush(void);
void	sub(void);
void	varpush(void);
void	varread(void);
void	verify(Symbol*);
void	whilecode(void);
void	xec(void);
void	xeff(void);
void	xpop(void);
void get(void);

double	Log(double), Log10(double), Gamma(double), Sqrt(double), Exp(double);
double	Asin(double), Acos(double), Sinh(double), Cosh(double), integer(double);
double	Pow(double, double);

void init(void);
void execerror(char*, char*);
void *emalloc(unsigned);

void	run(void);
void	warning(char*, char*);
Blk*	salloc(int size);
Blk*	morehd(void);
int	readc(void);
int	readd(void*);
void	unreadc(void);
void	more(Blk *hptr);
void	ospace(char *s);
void	garbage(char *s);
void	release(Blk *p);
void	pushio(Blk *p);

/* lexer tokens */
enum
{
	LXXX=256,
	LBLTIN,
	LFOR,
	LFUNC,
	LNOW,
	LNUMBER,
	LPRINT,
	LREAD,
	LRETURN,
	LSTOP,
	LSTRING,
	LUNDEF,
	LVAR,
	LWHILE,
	LOPS,
	LTEA,
};

/* Stack Types */
enum
{
	SNUM,
	SSTR,
	SFSTR,
	SSYM,
	SBLK,
};
