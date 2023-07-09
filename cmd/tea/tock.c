#include <u.h>
#include <libc.h>
#include <bio.h>

enum {MULTIPLIER = 31, NHASH = 126};
typedef struct Nameval Nameval;
struct Nameval 
{
	char	*name;
	float	value;
	ulong	time;
	Nameval	*next;
};
Nameval *symtab[NHASH];

int	rflag;
int	pflag=0;
Biobuf	bin;
Biobuf	bout;

int	debug=0;
Nameval * addsym(char *name, float val, ulong time);
uint hash(char *str);
void symloop(void);

void
main(int argc, char **argv)
{
	char *f[4];
	char *r;
	char buf[512];
	int	i;
	float	val;
	ulong	time;
	
	Binit(&bin, 0, OREAD);
	Binit(&bout, 1, OWRITE);
	ARGBEGIN {
	case 'r':
		rflag++;
		break;
	case 'p':
		pflag++;
		break;
	default:
		fprint(2, "%s: bad option %c\n", argv0, ARGC());
		exits("usage");
	} ARGEND
	while(r = Brdline(&bin, '\n')) {
		if(debug)
			print("flags rflag %d pflag %d\n", rflag, pflag);
		r[Blinelen(&bin) - 1] = 0;
		tokenize(r, f, 4);
		if(debug)
			fprint(2, "f1 %s f2 %s f3 %s\n", f[0], f[1], f[2]);
		strcpy(buf, f[1]);
		cleanname(buf);
		if(debug)
			fprint(2, "cleanname %s\n", buf);
		val=atof(f[2]);
		time=atol(f[0]);
		addsym(buf, val, time);
		if(rflag||pflag) {
			for (i = strlen(buf); i>=0; i--) {
				if(buf[i] == '/'){
					if(i == 0) {
						addsym("/", val, time);
					} else {
						buf[i] = '\0';
						addsym(buf, val, time);
					}
				}
			}
		}
	}
	symloop();
}

uint
hash(char *str)
{
	uint h;
	uchar *p;
	
	h = 0;
	for (p = (uchar *) str; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;
	return h % NHASH;
}

Nameval * 
addsym(char *name, float val, ulong time)
{
	int h;
	Nameval *sym;
	
	if(debug)
		fprint(2, "addsym %s\n", name);
	h = hash(name);
	for (sym = symtab[h]; sym; sym = sym->next)
		if(strcmp(name, sym->name) == 0)
			break;
	if (!sym) {
		sym = (Nameval *) malloc(sizeof(Nameval));
		sym->name = strdup(name);
		sym->value = 0;
		sym->time = 0;
		sym->next = symtab[h];
		symtab[h] = sym;
	}
	sym->value += val;
	sym->time = time;
	return sym;
}

int
symtime(char *name)
{
	int h;
	Nameval *sym;
	char parent[512];
	char *pr;
	
	strcpy(parent, name);
	pr = utfrrune(parent, '/');
	if(pr)
			*pr = 0;
	h = hash(parent);
	for (sym = symtab[h]; sym; sym = sym->next)
		if(strcmp(parent, sym->name) == 0)
			break;
	if(sym)
		return sym->value;
	else
		return 1;
}

void
symloop(void)
{
	int i;
	Nameval *sym;
	
	for(i = 0; i < NHASH; i++)
		for(sym = symtab[i]; sym; sym = sym->next)
			if(pflag)
				print("%ld\t%s\t%g\n", 
					sym->time, sym->name, sym->value/symtime(sym->name));
			else
				print("%ld\t%s\t%g\n", sym->time, sym->name, sym->value);
}
