#include	"grep.h"

void*
mal(int n)
{
	static char *s;
	static int m = 0;
	void *v;

	n = (n+3) & ~3;
	if(m < n) {
		if(n > Nhunk) {
			v = sbrk(n);
			memset(v, 0, n);
			return v;
		}
		s = sbrk(Nhunk);
		m = Nhunk;
	}
	v = s;
	s += n;
	m -= n;
	memset(v, 0, n);
	return v;
}

State*
sal(int n)
{
	State *s;

	s = mal(sizeof(*s));
	s->next = mal(256*sizeof(*s->next));
	s->count = n;
	s->re = mal(n*sizeof(*state0->re));
	return s;
}

Re*
ral(int type)
{
	Re *r;

	r = mal(sizeof(*r));
	r->type = type;
	maxfollow++;
	return r;
}

void
error(char *s)
{
	fprint(2, "grep: internal error: %s\n", s);
	exits(s);
}

int
countor(Re *r)
{
	int n;

	n = 0;
loop:
	switch(r->type) {
	case Tor:
		n += countor(r->_u.alt);
		r = r->next;
		goto loop;
	case Tclass:
		return n + r->_u._c.hi - r->_u._c.lo + 1;
	}
	return n;
}

Re*
oralloc(int t, Re *r, Re *b)
{
	Re *a;

	if(b == 0)
		return r;
	a = ral(t);
	a->_u.alt = r;
	a->next = b;
	return a;
}

void
case1(Re *c, Re *r)
{
	int n;

loop:
	switch(r->type) {
	case Tor:
		case1(c, r->_u.alt);
		r = r->next;
		goto loop;

	case Tclass:	/* add to character */
		for(n=r->_u._c.lo; n<=r->_u._c.hi; n++)
			c->_u.cases[n] = oralloc(Tor, r->next, c->_u.cases[n]);
		break;

	default:	/* add everything unknown to next */
		c->next = oralloc(Talt, r, c->next);
		break;
	}
}

Re*
addcase(Re *r)
{
	int i, n;
	Re *a;

	if(r->gen == gen)
		return r;
	r->gen = gen;
	switch(r->type) {
	default:
		error("addcase");

	case Tor:
		n = countor(r);
		if(n >= Caselim) {
			a = ral(Tcase);
			a->_u.cases = mal(256*sizeof(*a->_u.cases));
			case1(a, r);
			for(i=0; i<256; i++)
				if(a->_u.cases[i]) {
					r = a->_u.cases[i];
					if(countor(r) < n)
						a->_u.cases[i] = addcase(r);
				}
			return a;
		}
		return r;

	case Talt:
		r->next = addcase(r->next);
		r->_u.alt = addcase(r->_u.alt);
		return r;

	case Tbegin:
	case Tend:
	case Tclass:
		return r;
	}
}

void
str2top(char *p)
{
	Re2 oldtop;

	oldtop = topre;
	input = p;
	topre.beg = 0;
	topre.end = 0;
	yyparse();
	gen++;
	if(topre.beg == 0)
		yyerror("syntax");
	if(oldtop.beg)
		topre = re2or(oldtop, topre);
}

void
appendnext(Re *a, Re *b)
{
	Re *n;

	while(n = a->next)
		a = n;
	a->next = b;
}

void
patchnext(Re *a, Re *b)
{
	Re *n;

	while(a) {
		n = a->next;
		a->next = b;
		a = n;
	}
}

int
getrec(void)
{
	int c;

	if(flags['f']) {
		c = Bgetc(rein);
		if(c <= 0)
			return 0;
	} else
		c = *input++ & 0xff;
	if(flags['i'] && c >= 'A' && c <= 'Z')
		c += 'a'-'A';
	if(c == '\n')
		lineno++;
	return c;
}

Re2
re2cat(Re2 a, Re2 b)
{
	Re2 c;

	c.beg = a.beg;
	c.end = b.end;
	patchnext(a.end, b.beg);
	return c;
}

Re2
re2star(Re2 a)
{
	Re2 c;

	c.beg = ral(Talt);
	c.beg->_u.alt = a.beg;
	patchnext(a.end, c.beg);
	c.end = c.beg;
	return c;
}

Re2
re2or(Re2 a, Re2 b)
{
	Re2 c;

	c.beg = ral(Tor);
	c.beg->_u.alt = b.beg;
	c.beg->next = a.beg;
	c.end = b.end;
	appendnext(c.end,  a.end);
	return c;
}

Re2
re2char(int c0, int c1)
{
	Re2 c;

	c.beg = ral(Tclass);
	c.beg->_u._c.lo = c0 & 0xff;
	c.beg->_u._c.hi = c1 & 0xff;
	c.end = c.beg;
	return c;
}

void
reprint1(Re *a)
{
	int i, j;

loop:
	if(a == 0)
		return;
	if(a->gen == gen)
		return;
	a->gen = gen;
	print("%p: ", a);
	switch(a->type) {
	default:
		print("type %d\n", a->type);
		error("print1 type");

	case Tcase:
		print("case ->%p\n", a->next);
		for(i=0; i<256; i++)
			if(a->_u.cases[i]) {
				for(j=i+1; j<256; j++)
					if(a->_u.cases[i] != a->_u.cases[j])
						break;
				print("	[%.2x-%.2x] ->%p\n", i, j-1, a->_u.cases[i]);
				i = j-1;
			}
		for(i=0; i<256; i++)
			reprint1(a->_u.cases[i]);
		break;

	case Tbegin:
		print("^ ->%p\n", a->next);
		break;

	case Tend:
		print("$ ->%p\n", a->next);
		break;

	case Tclass:
		print("[%.2x-%.2x] ->%p\n", a->_u._c.lo, a->_u._c.hi, a->next);
		break;

	case Tor:
	case Talt:
		print("| %p ->%p\n", a->_u.alt, a->next);
		reprint1(a->_u.alt);
		break;
	}
	a = a->next;
	goto loop;
}

void
reprint(char *s, Re *r)
{
	print("%s:\n", s);
	gen++;
	reprint1(r);
	print("\n\n");
}
