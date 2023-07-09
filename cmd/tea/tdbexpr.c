#include "btree.h"

Biobuf	bin;
int	op;
Btree	bt;
Block	r;
char *key;
enum {ADD, MINUS, PRODUCT, DIVIDE};
double val;

void
usage(char *s)
{
	fprint(2, "usage: %s [-apmd] file key\n", s);
	exits("usage");
}
#define	BUFSIZE	1024
char	rest[BUFSIZE];

void
main(int argc, char *argv[]) 
{
	int n;
	char buf[BUFSIZE];
	char *f[4];
	char *s;
	long vt;
	int i;

	Binit(&bin, 0, OREAD);
	ARGBEGIN{
	case 'a':
		op = ADD;
		break;
	case 'm':
		op = MINUS;
		break;
	case 'p':
		op = PRODUCT;
		break;
	case 'd':
		op = DIVIDE;
		break;
	default:
		fprint(2, "%s: bad option %c\n", argv0, ARGC());
		usage(argv0);
	}ARGEND
	if(argc<1)
		usage(argv0);
	else if(argc == 1)
		key = 0;
	else if(argc == 2)
		key = argv[1];
	btree(&bt, argv[0], OREAD);
	while(s = Brdline(&bin, '\n')) {
		double x, y;
		s[Blinelen(&bin)-1] = 0;
		strncpy(buf, s, BUFSIZE);
		n = getfields(buf, f, 4, 0, "\t");
		if(n < 3) {
			print("%s\n", s);
		}else if(n > 3)
			strncpy(rest, f[3], BUFSIZE);
		else
			rest[0]=0;
		vt = atol(f[0]);
		x = atof(f[2]);
		r.m = 0;
		if(key)
			join(&bt, vt, key, f[2], 2, JSINGLE|JEXACT, &r);
		else
			join(&bt, vt, f[1], f[2], 2, JSINGLE|JEXACT, &r);
		for (i = 0; i < r.m; i++) {
			y = atof(r.ents[i]->val);
			switch(op) {
			case ADD:
				x +=  y;
				break;
			case MINUS:
				x -= y;
				break;
			case PRODUCT:
				x *= y;
				break;
			case DIVIDE:
				x /= y;
				break;
			}
			print("%ld\t%s\t%g\n", vt, f[1], x);
		}
	}
	exits(0);
}
