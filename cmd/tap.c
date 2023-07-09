#include <u.h>
#include <libc.h>
#include <bio.h>

int mflag;
int tflag;
int dflag;
int rflag;
int oflag;
int sflag;

typedef struct Tern Tern;
Struct Tern{
	char *aval;
	char *bval;
	ulong time;
	uchar toggle;
	Tern *next;
};


void
usage(void){
	fprint(2, "tap [-shntd] [file]\n");
	exit(1);
}

int main(int argc, char * argv[]) {
	int  f;
	char *b;
	int toggle;
	
	bsize = SIZE;
	b1=malloc(bsize);
	b2=malloc(bsize);
	f = 0;
	
	ARGBEGIN{
	case 'r':
		rflag=1;
		break;
	case 'o':
		oflag=1;
		break;
	case 'm':
		mflag=1;
		break;
	case 't':
		tflag=1;
		break;
	case 'd':
		dflag=1;
		break;
	case 'j':
		jflag=1;
		break;
	default:
		usage();
	}ARGEND
	if (argc == 1) {
		if((f=open(argv[0], OREAD))==0){
			fprint(2,"error opening file\n");
			exit(1);
		}
	}
	Binit(&bin, 0, OREAD);
	while(b = Brdline(&bin, '\n')) {
		Tern *l, *p, cur;
		mktern(&cur, b);
		l=lookup(cur.aval);
		if(tflag)
			for(p=l;p && p->t <= cur.t; p=p->next)
				p->toggle ^= 1;
		for (p=l; p; p=p->next)
			if(mflag)
				fmt(cur.t, cur.lval, cur.rval, p->t, cur.lval, p->rval);
			else if(tflag && p->toggle && p->t <= cur.t)
				fmt(cur.t, cur.lval, cur.rval, p->t, cur.lval, p->rval);
			else if(sflag && sjc(cur.t, p->t, (p->next?p->next->t:0));
		if(ret==0 && oflag)
			fmt(cur.t, cur.lval, cur.rval, 0, cur.lval, 0);
	}
	return 0;
}

int
sjc(ulong current, ulong a, ulong b)
{
	if(a==0)
		return 0;
	else if(b==0)
		if(a <= current)
			return 1;
		else
			return 0;
	else if(a <= current && current < b)
		return 1;
	else if(a <= current && current == b)
		return 1;
	else
		return 0;
}

void
fmt(ulong t1, char *a, char *b, ulong t2, char *c, char  *d)
{
	if(rflag){
		printf("%s%c%s%c%s%c", t1, OFS, a, OFS,  fn(d), (NF>3?OFS: "\n"));
	}else if(jflag){
		printf("%s%c%s%c%s%c%s%c", t1, OFS, a, OFS,  d, OFS, b, (NF>3?OFS: "\n"));
	}else{
		printf("%s%c%s%c%s%c", t1, OFS, d, OFS,  fn(b), (NF>3?OFS: "\n"));
	}
	for(i=4; i<=NF; i++)
		printf("%s%c", $i, (i==NF?"\n": OFS));
}

		