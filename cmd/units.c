
#line	2	"/home/xcs0998/src/cmd/units.y"
#include <u.h>
#include <libc.h>
#include <bio.h>

enum
{
	Ndim	= 15,		/* number of dimensions */
	Nsym	= 40,		/* size of a name */
	Nvar	= 203,		/* hash table size */
	Maxe	= 695,		/* log of largest number */
};

typedef	struct	Var	Var;
typedef	struct	Node	Node;
typedef	struct	Prefix	Prefix;

struct	Node
{
	double	val;
	signed char	dim[Ndim];
};
struct	Var
{
	Rune	name[Nsym];
	Node	node;
	Var*	link;
};
struct	Prefix
{
	double	val;
	Rune*	pname;
};

char	buf[100];
int	digval;
Biobuf*	fi;
Biobuf	linebuf;
Var*	fund[Ndim];
Rune	line[1000];
ulong	lineno;
int	linep;
int	nerrors;
Node	one;
int	peekrune;
Node	retnode1;
Node	retnode2;
Node	retnode;
Rune	sym[Nsym];
Var*	vars[Nvar];
int	vflag;

extern	void	add(Node*, Node*, Node*);
extern	void	div(Node*, Node*, Node*);
extern	int	specialcase(Node*, Node*, Node*);
extern	double	fadd(double, double);
extern	double	fdiv(double, double);
extern	double	fmul(double, double);
extern	int	gdigit(void*);
extern	Var*	lookup(int);
extern	void	main(int, char*[]);
extern	void	mul(Node*, Node*, Node*);
extern	void	ofile(void);
extern	double	pname(void);
extern	void	printdim(char*, int, int);
extern	int	ralpha(int);
extern	int	readline(void);
extern	void	sub(Node*, Node*, Node*);
extern	int	Uconv(va_list*, Fconv*);
extern	void	xpn(Node*, Node*, int);
extern	void	yyerror(char*, ...);
extern	int	yylex(void);
extern	int	yyparse(void);

typedef	Node*	indnode;
#pragma	varargck	type	"U"	indnode


#line	79	"/home/xcs0998/src/cmd/units.y"
typedef union 
{
	Node	node;
	Var*	var;
	int	numb;
	double	val;
} YYSTYPE;
extern	int	yyerrflag;
#ifndef	YYMAXDEPTH
#define	YYMAXDEPTH	150
#endif
YYSTYPE	yylval;
YYSTYPE	yyval;
#define	VAL	57346
#define	VAR	57347
#define	SUP	57348
#define YYEOFCODE 1
#define YYERRCODE 2

#line	217	"/home/xcs0998/src/cmd/units.y"


int
yylex(void)
{
	int c, i;

	c = peekrune;
	peekrune = ' ';

loop:
	if((c >= '0' && c <= '9') || c == '.')
		goto numb;
	if(ralpha(c))
		goto alpha;
	switch(c) {
	case ' ':
	case '\t':
		c = line[linep++];
		goto loop;
/*	case L'×':
		return '*';
	case L'÷':
		return '/';
	case L'¹':
		yylval.numb = 1;
		return SUP;
	case L'²':
		yylval.numb = 2;
		return SUP;
	case L'³':
		yylval.numb = 3;
		return SUP;
*/	}
	return c;

alpha:
	memset(sym, 0, sizeof(sym));
	for(i=0;; i++) {
		if(i < nelem(sym))
			sym[i] = c;
		c = line[linep++];
		if(!ralpha(c))
			break;
	}
	sym[nelem(sym)-1] = 0;
	peekrune = c;
	yylval.var = lookup(0);
	return VAR;

numb:
	digval = c;
	yylval.val = charstod(gdigit, 0);
	return VAL;
}

void
main(int argc, char *argv[])
{
	char *file;

	ARGBEGIN {
	default:
		print("usage: units [-v] [file]\n");
		exits("usage");
	case 'v':
		vflag = 1;
		break;
	} ARGEND

	file = "/lib/units";
	if(argc > 0)
		file = argv[0];
	fi = Bopen(file, OREAD);
	if(fi == 0) {
		print("cant open: %s\n", file);
		exits("open");
	}
	fmtinstall('U', Uconv);
	one.val = 1;

	/*
	 * read the 'units' file to
	 * develope a database
	 */
	lineno = 0;
	for(;;) {
		lineno++;
		if(readline())
			break;
		if(line[0] == 0 || line[0] == '/')
			continue;
		peekrune = ':';
		yyparse();
	}

	/*
	 * read the console to
	 * print ratio of pairs
	 */
	Bterm(fi);
	fi = &linebuf;
	Binit(fi, 0, OREAD);
	lineno = 0;
	for(;;) {
		if(lineno & 1)
			print("you want: ");
		else
			print("you have: ");
		if(readline())
			break;
		peekrune = '?';
		nerrors = 0;
		yyparse();
		if(nerrors)
			continue;
		if(lineno & 1) {
			if(specialcase(&retnode, &retnode2, &retnode1))
				print("\tis %U\n", &retnode);
			else {
				div(&retnode, &retnode2, &retnode1);
				print("\t* %U\n", &retnode);
				div(&retnode, &retnode1, &retnode2);
				print("\t/ %U\n", &retnode);
			}
		} else
			retnode2 = retnode1;
		lineno++;
	}
	print("\n");
	exits(0);
}

/*
 * all characters that have some
 * meaning. rest are usable as names
 */
int
ralpha(int c)
{
	switch(c) {
	case 0:
	case '+':
	case '-':
	case '*':
	case '/':
	case '[':
	case ']':
	case '(':
	case ')':
	case '^':
	case ':':
	case '?':
	case ' ':
	case '\t':
	case '.':
	case '|':
	case '#':
/*	case L'¹':
	case L'²':
	case L'³':
	case L'×':
	case L'÷':
*/		return 0;
	}
	return 1;
}

int
gdigit(void*z)
{
	int c;

	c = digval;
	if(c) {
		digval = 0;
		return c;
	}
	c = line[linep++];
	peekrune = c;
	return c;
}

void
yyerror(char *fmt, ...)
{
	va_list arg;

	/*
	 * hack to intercept message from yaccpar
	 */
	if(strcmp(fmt, "syntax error") == 0) {
		yyerror("syntax error, last name: %S", sym);
		return;
	}
	va_start(arg, fmt);
	doprint(buf, buf+sizeof(buf), fmt, arg);
	va_end(arg);
	print("%ld: %S\n\t%s\n", lineno, line, buf);
	nerrors++;
	if(nerrors > 5) {
		print("too many errors\n");
		exits("errors");
	}
}

void
add(Node *c, Node *a, Node *b)
{
	int i, d;

	for(i=0; i<Ndim; i++) {
		d = a->dim[i];
		c->dim[i] = d;
		if(d != b->dim[i])
			yyerror("add must be like units");
	}
	c->val = fadd(a->val, b->val);
}

void
sub(Node *c, Node *a, Node *b)
{
	int i, d;

	for(i=0; i<Ndim; i++) {
		d = a->dim[i];
		c->dim[i] = d;
		if(d != b->dim[i])
			yyerror("sub must be like units");
	}
	c->val = fadd(a->val, -b->val);
}

void
mul(Node *c, Node *a, Node *b)
{
	int i;

	for(i=0; i<Ndim; i++)
		c->dim[i] = a->dim[i] + b->dim[i];
	c->val = fmul(a->val, b->val);
}

void
div(Node *c, Node *a, Node *b)
{
	int i;

	for(i=0; i<Ndim; i++)
		c->dim[i] = a->dim[i] - b->dim[i];
	c->val = fdiv(a->val, b->val);
}

void
xpn(Node *c, Node *a, int b)
{
	int i;

	*c = one;
	if(b < 0) {
		b = -b;
		for(i=0; i<b; i++)
			div(c, c, a);
	} else
	for(i=0; i<b; i++)
		mul(c, c, a);
}

int
specialcase(Node *c, Node *a, Node *b)
{
	int i, d, d1, d2;

	d1 = 0;
	d2 = 0;
	for(i=1; i<Ndim; i++) {
		d = a->dim[i];
		if(d) {
			if(d != 1 || d1)
				return 0;
			d1 = i;
		}
		d = b->dim[i];
		if(d) {
			if(d != 1 || d2)
				return 0;
			d2 = i;
		}
	}
	if(d1 == 0 || d2 == 0)
		return 0;

	if(memcmp(fund[d1]->name, L"°C", 3*sizeof(Rune)) == 0 &&
	   memcmp(fund[d2]->name, L"°F", 3*sizeof(Rune)) == 0 &&
	   b->val == 1) {
		memcpy(c->dim, b->dim, sizeof(c->dim));
		c->val = a->val * 9. / 5. + 32.;
		return 1;
	}

	if(memcmp(fund[d1]->name, L"°F", 3*sizeof(Rune)) == 0 &&
	   memcmp(fund[d2]->name, L"°C", 3*sizeof(Rune)) == 0 &&
	   b->val == 1) {
		memcpy(c->dim, b->dim, sizeof(c->dim));
		c->val = (a->val - 32.) * 5. / 9.;
		return 1;
	}
	return 0;
}

void
printdim(char *str, int d, int n)
{
	Var *v;

	if(n) {
		v = fund[d];
		if(v)
			sprint(strchr(str, 0), " %S", v->name);
		else
			sprint(strchr(str, 0), " [%d]", d);
		switch(n) {
		case 1:
			break;
		case 2:
			strcat(str, "²");
			break;
		case 3:
			strcat(str, "³");
			break;
		default:
			sprint(strchr(str, 0), "^%d", n);
		}
	}
}

int
Uconv(va_list *arg, Fconv *fp)
{
	char str[200];
	Node *n;
	int f, i, d;

	n = va_arg(*arg, Node*);
	sprint(str, "%g", n->val);

	f = 0;
	for(i=1; i<Ndim; i++) {
		d = n->dim[i];
		if(d > 0)
			printdim(str, i, d);
		else
		if(d < 0)
			f = 1;
	}

	if(f) {
		strcat(str, " /");
		for(i=1; i<Ndim; i++) {
			d = n->dim[i];
			if(d < 0)
				printdim(str, i, -d);
		}
	}

	strconv(str, fp);
	return sizeof n;
}

int
readline(void)
{
	int i, c;

	linep = 0;
	for(i=0;; i++) {
		c = Bgetrune(fi);
		if(c < 0)
			return 1;
		if(c == '\n')
			break;
		if(i < nelem(line))
			line[i] = c;
	}
	if(i >= nelem(line))
		i = nelem(line)-1;
	line[i] = 0;
	return 0;
}

Var*
lookup(int f)
{
	int i;
	Var *v, *w;
	double p;
	ulong h;

	h = 0;
	for(i=0; sym[i]; i++)
		h = h*13 + sym[i];
	h %= nelem(vars);

	for(v=vars[h]; v; v=v->link)
		if(memcmp(sym, v->name, sizeof(sym)) == 0)
			return v;
	if(f)
		return 0;
	v = malloc(sizeof(*v));
	if(v == nil) {
		fprint(2, "out of memory\n");
		exits("mem");
	}
	memset(v, 0, sizeof(*v));
	memcpy(v->name, sym, sizeof(sym));
	v->link = vars[h];
	vars[h] = v;

	p = 1;
	for(;;) {
		p = fmul(p, pname());
		if(p == 0)
			break;
		w = lookup(1);
		if(w) {
			v->node = w->node;
			v->node.val = fmul(v->node.val, p);
			break;
		}
	}
	return v;
}

Prefix	prefix[] =
{
	1e-24,	L"yocto",
	1e-21,	L"zepto",
	1e-18,	L"atto",
	1e-15,	L"femto",
	1e-12,	L"pico",
	1e-9,	L"nano",
	1e-6,	L"micro",
	1e-6,	L"μ",
	1e-3,	L"milli",
	1e-2,	L"centi",
	1e-1,	L"deci",
	1e1,	L"deka",
	1e2,	L"hecta",
	1e2,	L"hecto",
	1e3,	L"kilo",
	1e6,	L"mega",
	1e6,	L"meg",
	1e9,	L"giga",
	1e12,	L"tera",
	1e15,	L"peta",
	1e18,	L"exa",
	1e21,	L"zetta",
	1e24,	L"yotta",
	0,	0
};

double
pname(void)
{
	Rune *p;
	int i, j, c;

	/*
	 * rip off normal prefixs
	 */
	for(i=0; p=prefix[i].pname; i++) {
		for(j=0; c=p[j]; j++)
			if(c != sym[j])
				goto no;
		memmove(sym, sym+j, (Nsym-j)*sizeof(*sym));
		memset(sym+(Nsym-j), 0, j*sizeof(*sym));
		return prefix[i].val;
	no:;
	}

	/*
	 * rip off 's' suffixes
	 */
	for(j=0; sym[j]; j++)
		;
	j--;
	/* j>1 is special hack to disallow ms finding m */
	if(j > 1 && sym[j] == 's') {
		sym[j] = 0;
		return 1;
	}
	return 0;
}

/*
 * careful floating point
 */
double
fmul(double a, double b)
{
	double l;

	if(a <= 0) {
		if(a == 0)
			return 0;
		l = log(-a);
	} else
		l = log(a);

	if(b <= 0) {
		if(b == 0)
			return 0;
		l += log(-b);
	} else
		l += log(b);

	if(l > Maxe) {
		yyerror("overflow in multiply");
		return 1;
	}
	if(l < -Maxe) {
		yyerror("underflow in multiply");
		return 0;
	}
	return a*b;
}

double
fdiv(double a, double b)
{
	double l;

	if(a <= 0) {
		if(a == 0)
			return 0;
		l = log(-a);
	} else
		l = log(a);

	if(b <= 0) {
		if(b == 0) {
			yyerror("division by zero");
			return 1;
		}
		l -= log(-b);
	} else
		l -= log(b);

	if(l > Maxe) {
		yyerror("overflow in divide");
		return 1;
	}
	if(l < -Maxe) {
		yyerror("underflow in divide");
		return 0;
	}
	return a/b;
}

double
fadd(double a, double b)
{
	return a + b;
}
short	yyexca[] =
{-1, 1,
	1, -1,
	-2, 0,
};
#define	YYNPROD	21
#define	YYPRIVATE 57344
#define	YYLAST	41
short	yyact[] =
{
   8,  10,   7,   9,  16,  17,  12,  11,  20,  21,
  15,  31,  23,   6,   4,  12,  11,  22,  13,   5,
   1,  27,  28,   0,  14,  30,  29,  13,  20,  20,
  25,  26,   0,  24,  18,  19,  16,  17,   2,   0,
   3
};
short	yypact[] =
{
  31,-1000,   9,  11,   2,  26,  22,  11,   3,  -3,
-1000,-1000,-1000,  11,  26,-1000,  11,  11,  11,  11,
   3,-1000,  11,  11,  -6,  22,  22,  11,  11,  -3,
-1000,-1000
};
short	yypgo[] =
{
   0,  20,  19,   1,   3,   0,   2,  13
};
short	yyr1[] =
{
   0,   1,   1,   1,   1,   2,   2,   2,   7,   7,
   7,   6,   6,   5,   5,   5,   4,   4,   3,   3,
   3
};
short	yyr2[] =
{
   0,   3,   3,   2,   1,   1,   3,   3,   1,   3,
   3,   1,   2,   1,   2,   3,   1,   3,   1,   1,
   3
};
short	yychk[] =
{
-1000,  -1,   7,   9,   5,  -2,  -7,  -6,  -5,  -4,
  -3,   5,   4,  16,  -2,   8,  10,  11,  12,  13,
  -5,   6,  14,  15,  -2,  -7,  -7,  -6,  -6,  -4,
  -3,  17
};
short	yydef[] =
{
   0,  -2,   0,   4,   0,   3,   5,   8,  11,  13,
  16,  18,  19,   0,   1,   2,   0,   0,   0,   0,
  12,  14,   0,   0,   0,   6,   7,   9,  10,  15,
  17,  20
};
short	yytok1[] =
{
   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   8,   0,   0,   0,   0,
  16,  17,  12,  10,   0,  11,   0,  13,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   7,   0,
   0,   0,   0,   9,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  14,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  15
};
short	yytok2[] =
{
   2,   3,   4,   5,   6
};
long	yytok3[] =
{
   0
};
#define YYFLAG 		-1000
#define	yyclearin	yychar = -1
#define	yyerrok		yyerrflag = 0

#ifdef	yydebug
#include	"y.debug"
#else
#define	yydebug		0
char*	yytoknames[1];		/* for debugging */
char*	yystates[1];		/* for debugging */
#endif

/*	parser for yacc output	*/

int	yynerrs = 0;		/* number of errors */
int	yyerrflag = 0;		/* error recovery flag */


char*
yytokname(int yyc)
{
	static char x[10];

	if(yyc > 0 && yyc <= sizeof(yytoknames)/sizeof(yytoknames[0]))
	if(yytoknames[yyc-1])
		return yytoknames[yyc-1];
	sprint(x, "<%d>", yyc);
	return x;
}

char*
yystatname(int yys)
{
	static char x[10];

	if(yys >= 0 && yys < sizeof(yystates)/sizeof(yystates[0]))
	if(yystates[yys])
		return yystates[yys];
	sprint(x, "<%d>\n", yys);
	return x;
}

long
yylex1(void)
{
	long yychar;
	long *t3p;
	int c;

	yychar = yylex();
	if(yychar <= 0) {
		c = yytok1[0];
		goto out;
	}
	if(yychar < sizeof(yytok1)/sizeof(yytok1[0])) {
		c = yytok1[yychar];
		goto out;
	}
	if(yychar >= YYPRIVATE)
		if(yychar < YYPRIVATE+sizeof(yytok2)/sizeof(yytok2[0])) {
			c = yytok2[yychar-YYPRIVATE];
			goto out;
		}
	for(t3p=yytok3;; t3p+=2) {
		c = t3p[0];
		if(c == yychar) {
			c = t3p[1];
			goto out;
		}
		if(c == 0)
			break;
	}
	c = 0;

out:
	if(c == 0)
		c = yytok2[1];	/* unknown char */
	if(yydebug >= 3)
		fprint(2, "lex %.4lux %s\n", yychar, yytokname(c));
	return c;
}

int
yyparse(void)
{
	struct
	{
		YYSTYPE	yyv;
		int	yys;
	} yys[YYMAXDEPTH], *yyp, *yypt;
	short *yyxi;
	int yyj, yym, yystate, yyn, yyg;
	long yychar;
	YYSTYPE save1, save2;
	int save3, save4;

	save1 = yylval;
	save2 = yyval;
	save3 = yynerrs;
	save4 = yyerrflag;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyp = &yys[-1];
	goto yystack;

ret0:
	yyn = 0;
	goto ret;

ret1:
	yyn = 1;
	goto ret;

ret:
	yylval = save1;
	yyval = save2;
	yynerrs = save3;
	yyerrflag = save4;
	return yyn;

yystack:
	/* put a state and value onto the stack */
	if(yydebug >= 4)
		fprint(2, "char %s in %s", yytokname(yychar), yystatname(yystate));

	yyp++;
	if(yyp >= &yys[YYMAXDEPTH]) {
		yyerror("yacc stack overflow");
		goto ret1;
	}
	yyp->yys = yystate;
	yyp->yyv = yyval;

yynewstate:
	yyn = yypact[yystate];
	if(yyn <= YYFLAG)
		goto yydefault; /* simple state */
	if(yychar < 0)
		yychar = yylex1();
	yyn += yychar;
	if(yyn < 0 || yyn >= YYLAST)
		goto yydefault;
	yyn = yyact[yyn];
	if(yychk[yyn] == yychar) { /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if(yyerrflag > 0)
			yyerrflag--;
		goto yystack;
	}

yydefault:
	/* default state action */
	yyn = yydef[yystate];
	if(yyn == -2) {
		if(yychar < 0)
			yychar = yylex1();

		/* look through exception table */
		for(yyxi=yyexca;; yyxi+=2)
			if(yyxi[0] == -1 && yyxi[1] == yystate)
				break;
		for(yyxi += 2;; yyxi += 2) {
			yyn = yyxi[0];
			if(yyn < 0 || yyn == yychar)
				break;
		}
		yyn = yyxi[1];
		if(yyn < 0)
			goto ret0;
	}
	if(yyn == 0) {
		/* error ... attempt to resume parsing */
		switch(yyerrflag) {
		case 0:   /* brand new error */
			yyerror("syntax error");
			yynerrs++;
			if(yydebug >= 1) {
				fprint(2, "%s", yystatname(yystate));
				fprint(2, "saw %s\n", yytokname(yychar));
			}

		case 1:
		case 2: /* incompletely recovered error ... try again */
			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */
			while(yyp >= yys) {
				yyn = yypact[yyp->yys] + YYERRCODE;
				if(yyn >= 0 && yyn < YYLAST) {
					yystate = yyact[yyn];  /* simulate a shift of "error" */
					if(yychk[yystate] == YYERRCODE)
						goto yystack;
				}

				/* the current yyp has no shift onn "error", pop stack */
				if(yydebug >= 2)
					fprint(2, "error recovery pops state %d, uncovers %d\n",
						yyp->yys, (yyp-1)->yys );
				yyp--;
			}
			/* there is no state on the stack with an error shift ... abort */
			goto ret1;

		case 3:  /* no shift yet; clobber input char */
			if(yydebug >= 2)
				fprint(2, "error recovery discards %s\n", yytokname(yychar));
			if(yychar == YYEOFCODE)
				goto ret1;
			yychar = -1;
			goto yynewstate;   /* try again in the same state */
		}
	}

	/* reduction by production yyn */
	if(yydebug >= 2)
		fprint(2, "reduce %d in:\n\t%s", yyn, yystatname(yystate));

	yypt = yyp;
	yyp -= yyr2[yyn];
	yyval = (yyp+1)->yyv;
	yym = yyn;

	/* consult goto table to find next state */
	yyn = yyr1[yyn];
	yyg = yypgo[yyn];
	yyj = yyg + yyp->yys + 1;

	if(yyj >= YYLAST || yychk[yystate=yyact[yyj]] != -yyn)
		yystate = yyact[yyg];
	switch(yym) {
		
case 1:
#line	95	"/home/xcs0998/src/cmd/units.y"
{
		int f;

		f = yypt[-1].yyv.var->node.dim[0];
		yypt[-1].yyv.var->node = yypt[-0].yyv.node;
		yypt[-1].yyv.var->node.dim[0] = 1;
		if(f)
			yyerror("redefinition of %S", yypt[-1].yyv.var->name);
		else
		if(vflag)
			print("%S\t%U\n", yypt[-1].yyv.var->name, &yypt[-1].yyv.var->node);
	} break;
case 2:
#line	108	"/home/xcs0998/src/cmd/units.y"
{
		int f, i;

		for(i=1; i<Ndim; i++)
			if(fund[i] == 0)
				break;
		if(i >= Ndim) {
			yyerror("too many dimensions");
			i = Ndim-1;
		}
		fund[i] = yypt[-1].yyv.var;

		f = yypt[-1].yyv.var->node.dim[0];
		yypt[-1].yyv.var->node = one;
		yypt[-1].yyv.var->node.dim[0] = 1;
		yypt[-1].yyv.var->node.dim[i] = 1;
		if(f)
			yyerror("redefinition of %S", yypt[-1].yyv.var->name);
		else
		if(vflag)
			print("%S\t#\n", yypt[-1].yyv.var->name);
	} break;
case 3:
#line	131	"/home/xcs0998/src/cmd/units.y"
{
		retnode1 = yypt[-0].yyv.node;
	} break;
case 4:
#line	135	"/home/xcs0998/src/cmd/units.y"
{
		retnode1 = one;
	} break;
case 6:
#line	142	"/home/xcs0998/src/cmd/units.y"
{
		add(&yyval.node, &yypt[-2].yyv.node, &yypt[-0].yyv.node);
	} break;
case 7:
#line	146	"/home/xcs0998/src/cmd/units.y"
{
		sub(&yyval.node, &yypt[-2].yyv.node, &yypt[-0].yyv.node);
	} break;
case 9:
#line	153	"/home/xcs0998/src/cmd/units.y"
{
		mul(&yyval.node, &yypt[-2].yyv.node, &yypt[-0].yyv.node);
	} break;
case 10:
#line	157	"/home/xcs0998/src/cmd/units.y"
{
		div(&yyval.node, &yypt[-2].yyv.node, &yypt[-0].yyv.node);
	} break;
case 12:
#line	164	"/home/xcs0998/src/cmd/units.y"
{
		mul(&yyval.node, &yypt[-1].yyv.node, &yypt[-0].yyv.node);
	} break;
case 14:
#line	171	"/home/xcs0998/src/cmd/units.y"
{
		xpn(&yyval.node, &yypt[-1].yyv.node, yypt[-0].yyv.numb);
	} break;
case 15:
#line	175	"/home/xcs0998/src/cmd/units.y"
{
		int i;

		for(i=1; i<Ndim; i++)
			if(yypt[-0].yyv.node.dim[i]) {
				yyerror("exponent has units");
				yyval.node = yypt[-2].yyv.node;
				break;
			}
		if(i >= Ndim) {
			i = yypt[-0].yyv.node.val;
			if(i != yypt[-0].yyv.node.val)
				yyerror("exponent not integral");
			xpn(&yyval.node, &yypt[-2].yyv.node, i);
		}
	} break;
case 17:
{
		div(&yyval.node, &yypt[-2].yyv.node, &yypt[-0].yyv.node);
	} break;
case 18:
#line	201	"/home/xcs0998/src/cmd/units.y"
{
		if(yypt[-0].yyv.var->node.dim[0] == 0) {
			yyerror("undefined %S", yypt[-0].yyv.var->name);
			yyval.node = one;
		} else
			yyval.node = yypt[-0].yyv.var->node;
	} break;
case 19:
#line	209	"/home/xcs0998/src/cmd/units.y"
{
		yyval.node = one;
		yyval.node.val = yypt[-0].yyv.val;
	} break;
case 20:
#line	214	"/home/xcs0998/src/cmd/units.y"
{
		yyval.node = yypt[-1].yyv.node;
	} break;
	}
	goto yystack;  /* stack new state and value */
}
