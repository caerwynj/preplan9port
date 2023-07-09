#include "btree.h"

Biobuf	bin;
int	xflag;
Btree	bt;

void
usage(char *s)
{
	fprint(2, "usage: %s [-x123] file [start] [end]\n", s);
	exits("usage");
}
#define	BUFSIZE	1024

void
main(int argc, char *argv[]) 
{
	int n;
	char str[BUFSIZE];
	char *f[4];
	char *s;
	int i = 0;
	int first=0;

	Binit(&bin, 0, OREAD);
	ARGBEGIN{
	case '1':
	case '2':
	case '3':
		i = ARGC() - '0';
		break;
	case 'x':
		xflag++;
		break;
	default:
		fprint(2, "%s: bad option %c\n", argv0, ARGC());
		usage(argv0);
	}ARGEND
	if(argc<1)
		usage(argv0);
	btree(&bt, argv[0], OREAD);
	if (s = getenv("ETT"))
		bt.ett = atol(s);
	else
		bt.ett = 0;		/* 0 is now */
	if(argc == 1) {
		while(s = Brdline(&bin, '\n')) {
			s[Blinelen(&bin)-1] = 0;
			strncpy(str, s, BUFSIZE);
			n = getfields(str, f, 2, 0, " \n\t");
			if(first)
				print("\n");
			first++;
			if(debug)
				fprint(2, "n = %d, %s, %s\n", n, f[0], f[1]);
			if(xflag && n > 0)
				search(&bt, f[0], f[0], i);
			else if(n == 1)
				search(&bt, f[0], 0, i);
			else if(n > 1)
				search(&bt, f[0], f[1], i);
		}
	} else if(argc == 3)
		search(&bt, argv[1], argv[2], i);
	else if(xflag)
		search(&bt, argv[1], argv[1], i);
	else
		search(&bt, argv[1], 0, i);
	exits(0);
}
