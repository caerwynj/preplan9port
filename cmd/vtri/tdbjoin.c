#include "btree.h"

#define	BUFSIZE	1024
Biobuf	bin;
Biobuf	bout;
Btree	bt;
char	rest[BUFSIZE];
int	ret;
int	iflag=2;
int	sflag;
int	vflag;
int	jflag;
int	oflag;
Block r;

void	fmt(ulong, char*, char*, ulong, char*, char*, int);

void
usage(char *s)
{
	fprint(2, "usage: %s [-mvsjo] file\n", s);
	exits("usage");
}

void
main(int argc, char *argv[]) 
{
	int n;
	char buf[BUFSIZE];
	char *f[4];
	char *s;
	long vt;
	int flags = JXXX;
	int i;
	
	Binit(&bin, 0, OREAD);
	Binit(&bout, 1, OWRITE);
	ARGBEGIN{
	case '1':
	case '2':
	case '3':
		iflag = ARGC() - '0';
		break;
	case 'j':
		jflag++;
		break;
	case 'm':
		flags |= JMANY;
		break;
	case 'o':
		oflag++;
		break;
	case 'p':
		flags |= JPREFIX;
		break;
	case 'r':
		flags |= JRANGE;
		break;
	case 's':
		sflag++;
		break;
	case 't':
		flags |= JTOGGLE;
		break;
	case 'v':
		vflag++;
		break;
	default:
		fprint(2, "%s: bad option %c\n", argv0, ARGC());
		usage(argv0);
	}ARGEND
	if(argc<1)
		usage(argv0);
	if(!(flags & (JMANY | JTOGGLE)))
		flags |= JSINGLE;
	if(!(flags & (JRANGE | JPREFIX)))
		flags |= JEXACT;
	btree(&bt, *argv, OREAD);
	if (s = getenv("ETT"))
		bt.ett = atol(s);
	else
		bt.ett = time(0);
	while(s = Brdline(&bin, '\n')) {
		s[Blinelen(&bin)-1] = 0;
		strncpy(buf, s, BUFSIZE);
		n = getfields(buf, f, 4, 0, "\t");
		if(n < 3) {
			Bprint(&bout, "%s\n", s);
		}else if(n > 3)
			strncpy(rest, f[3], BUFSIZE);
		else
			rest[0]=0;
		vt = atol(f[0]);
		r.m = 0;
		join(&bt, vt, f[1], f[2], iflag, flags, &r);
		for(i = 0; i < r.m; i++) {
			fmt(vt, f[1], f[2], r.ents[i]->vt, r.ents[i]->key, r.ents[i]->val, r.m);
		}
		if(r.m == 0 && oflag)
			fmt(vt, f[1], f[2], vt, f[1], "", r.m);
	}
	exits(0);
}

void
fmt(ulong tt, char *a1, char *b1, ulong vt, char *a2, char *b2, int n)
{
	ulong time;
	ret++;
	time = vflag ? vt : tt;
	if(sflag)
		Bprint(&bout, "%ld\t%s\t%s", time, b2, b1);
	else if(iflag == 3)
		Bprint(&bout, "%ld\t%s\t%s", time, b2, a2);
	else if(iflag == 1)
		Bprint(&bout, "%ld\t%s\t%s", time, a2, b2);
	else if(jflag)
		Bprint(&bout, "%ld\t%s\t%s\t%s", time, a1, b2, b1);
	else
		Bprint(&bout, "%ld\t%s\t%s", time, a2, b2);
	if(rest[0])
		Bprint(&bout, "\t%s\n", rest);
	else
		Bprint(&bout, "\n");
	USED(n);
}
