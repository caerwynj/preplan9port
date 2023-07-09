#include "btree.h"

typedef	struct	Sym	Sym;

#define	ETINIT	1
#define	ETGROW	2
#define	MULT	31
#define	NHASH	126

struct	Etab
{
	int	nval;
	int	max;
	Entry**	e;
} etab;

struct	Sym 
{
	char*	key;
	char*	val;
	int	value;
	Sym*	next;
} *symtab[NHASH];


ulong	evt;
int	tflag;
int	addet(Entry*);
void	clearhash(void);
uint	hash(Entry*);
int	looktog(Entry*, int);
int	sjc(long current, long pair[]);

int
addet(Entry *e)
{
	Entry ** evp;
	if(debug)
		fprint(2, "addet %ld %s %s\n", e->vt, e->key, e->val);
	if(etab.e == 0) {
		etab.e = (Entry **) malloc(ETINIT * sizeof(Entry *));
		if(etab.e == 0)
			return -1;
		etab.max = ETINIT;
		etab.nval = 0;
	}else if(etab.nval >= etab.max) {  /*grow*/
		evp = (Entry **) realloc(etab.e, (ETGROW * etab.max) * sizeof(Entry *));
		if(evp == 0)
			return -1;
		etab.max *= ETGROW;
		etab.e = evp;
	}
	if(tflag && e->vt <= evt)
		looktog(e, 1);
	etab.e[etab.nval] = e;
	return etab.nval++;
}


uint
hash(Entry *e)
{
	uint h;
	uchar *p;
	
	h = 0;
	for(p = (uchar *) e->key; *p != '\0'; p++)
		h = MULT * h + *p;
	for(p = (uchar *) e->val; *p != '\0'; p++)
		h = MULT * h + *p;
	return h % NHASH;
}

int 
looktog(Entry *e, int tog)
{
	int h;
	Sym *sym;
	
	h = hash(e);
	for(sym = symtab[h]; sym; sym = sym->next)
		if((e->key == sym->key && e->val == sym->val) || 
			(strcmp(e->key, sym->key) == 0 && strcmp(e->val, sym->val) == 0))
			break;
	if(!sym) {
		sym = (Sym *) malloc(sizeof(Sym));
		sym->key = e->key;
		sym->val = e->val;
		sym->value = 0;
		sym->next = symtab[h];
		symtab[h] = sym;
	}
	if(tog)
		sym->value ^= 1;
	return sym->value;
}

void
clearhash(void)
{
	Sym *sym, *next;
	int i;
	
	for(i = 0; i < NHASH; i++) {
		for(sym = symtab[i]; sym; sym = next) {
			next = sym->next;
			free(sym);
		}
		symtab[i] = 0;
	}
}

int
join(Btree *b, ulong vt, char *s, char *t, int inv, int flags, Block *r)
{
	int j;
	long pair[2];

	USED(t);
	b->fn = addet;
	tflag = flags & JTOGGLE;
	evt = vt;
	pair[0] = pair[1] = -1;
	if(flags & JRANGE)
		search(b, s, t, inv);
	else if(flags & JEXACT)
		search(b, s, s, inv);
	else if(flags & JPREFIX)
		search(b, s, 0, inv);
	if(debug)
		fprint(2, "nval %d\n", etab.nval);
	for(j = 0; j < etab.nval; j++) {
		if(flags & JMANY) {
			addentry(r, etab.e[j], r->m, 0);
		} else if(flags & JTOGGLE) {
			if(looktog(etab.e[j], 0) && etab.e[j]->vt <= vt) {
				addentry(r, etab.e[j], r->m, 0);
			}
		} else if(flags & JSINGLE) {
			pair[0] = etab.e[j]->vt;
			if(j == (etab.nval - 1))
				pair[1] = -1;
			else if (inv == 2 && (etab.e[j]->key == etab.e[j + 1]->key 
				|| strcmp(etab.e[j]->key, etab.e[j + 1]->key) == 0))
				pair[1] = etab.e[j + 1]->vt;
			else if (inv == 3 && (etab.e[j]->val == etab.e[j + 1]->val 
				|| strcmp(etab.e[j]->val, etab.e[j + 1]->val) == 0))
				pair[1] = etab.e[j + 1]->vt;
			else
				pair[1] = -1;
			if(sjc(vt, pair))
				addentry(r, etab.e[j], r->m, 0);
		}
	}
	etab.nval=0;
	if(flags & JTOGGLE)
		clearhash();
	return r->m;
}

int
sjc(long current, long pair[]) 
{
	if(debug)
		fprint(2, "sjc %ld %ld %ld\n", pair[0], current, pair[1]);
	if(pair[0] == -1)
		return 0;
	else if(pair[1] == -1)
		if(pair[0] <= current)
			return 1;
		else
			return 0;
	else if(pair[0] <= current && current < pair[1])
		return 1;
	else if(pair[0] == current && current == pair[1])
		return 1;
	else
		return 0;
}
