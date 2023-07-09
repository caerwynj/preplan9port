#include "btree.h"

Biobuf	bin;
Btree	bt;

void
usage(char *s)
{
	fprint(2, "usage: %s [-123p] file [value]\n", s);
	exits("usage");
}

int pflag;

void
main(int argc, char *argv[]) 
{
	int l;
	char *f[4];
	char *s, *k, *v;
	int n;
	int i;
	ulong tt, vt;
	int inv[4] = {0, 0, 0, 0};

	Binit(&bin, 0, OREAD);
	ARGBEGIN{
	case '1':
		inv[1]=1;
		break;
	case '2':
		inv[2]=1;
		break;
	case '3':
		inv[3]=1;
		break;
	case 'p':
		pflag++;
		break;
	default:
		fprint(2, "%s: bad option %c\n", argv0, ARGC());
		usage(argv0);
	}ARGEND
	if(argc<1)
		usage(argv0);
	btree(&bt, *argv, ORDWR);
	while(s = Brdline(&bin, '\n')) {
		l = Blinelen(&bin);
		s[l-1] = '\0';
		n = getfields(s, f, 4, 0, "\t\n");
		switch(n){
		case 0:
			continue;
		case 1:
			tt = vt = time(0);
			k = f[0];
			v = "";
			break;
		case 2:
			tt = vt = time(0);
			k = f[0];
			v = f[1];
			break;
		case 3:
			tt = time(0);
			vt = atoi(f[0]);
			k = f[1];
			v = f[2];
			break;
		case 4:
		default:
			tt = atoi(f[0]);
			vt = atoi(f[1]);
			k = f[2];
			v = f[3];
			break;
		}
		if(argc == 2)
			v = argv[1];
		for(i = 0; i < 4; i++)
			if(inv[i])
				insert(&bt, tt, vt, k, v, i);
		if(pflag)
			print("%ld\t%s\t%s\n", vt, k, v);
	}
	bflush(&bt);
	exits(0);
}
