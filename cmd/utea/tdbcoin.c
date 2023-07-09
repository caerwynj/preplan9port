#include "btree.h"

Biobuf	bin;
Biobuf	bout;
Btree	bt;

void
usage(char *s)
{
	fprint(2, "usage: %s [-123] file\n", s);
	exits("usage");
}

int dflag;

void
main(int argc, char *argv[]) 
{
	int l;
	char *f[4];
	char *s;
	ulong vt;
	int inv = 1;
	Entry *e;

	Binit(&bin, 0, OREAD);
	Binit(&bout, 1, OWRITE);
	ARGBEGIN{
	case '1': inv=1; break;
	case '2': inv=2; break;
	case '3': inv=3; break;
	case 'd': dflag++; break;
	default:
		fprint(2, "%s: bad option %c\n", argv0, ARGC());
		usage(argv0);
	}ARGEND
	if(argc<1)
		usage(argv0);
	btree(&bt, *argv, OREAD);
	while(s = Brdline(&bin, '\n')) {
		l = Blinelen(&bin);
		s[l-1] = '\0';
		getfields(s, f, 4, 0, "\t\n");
		vt = atoi(f[0]);
		e = coin(&bt, vt, f[1], 0, inv, dflag);
		if(e)
			Bprint(&bout, "%ld\t%s\t%s\n", e->vt, e->key, e->val);
	}
	exits(0);
}
