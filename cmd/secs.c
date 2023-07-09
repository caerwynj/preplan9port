#include <u.h>
#include <libc.h>
#include <bio.h>

#define	SIZE	1000
Biobuf	bin;
enum {
	SUM,
	HRS,
	SECS,
	TM,
	LOCAL
};

char *b1, *b2;
long bsize, fmt=TM;
void prtime(char *);

void usage(void){
	fprint(2, "secs [-shntd] [file]\n");
	exit(1);
}

int
gline(char *buf)
{
	char *p;
	int len;

	p = Brdline(&bin, '\n');
	if(p == 0)
		return 1;
	len = Blinelen(&bin);
	if(len >= bsize-1) {
		fprint(2, "line too long\n");
		exits("too long");
	}
	memmove(buf, p, len);
	buf[len] = 0;
	return 0;
}

int main(int argc, char * argv[]) {
	int  f;
	char *b;
	ulong  end=1;
	char *fld1[3];
	char *fld2[3];
	int toggle;
	
	bsize = SIZE;
	b1=malloc(bsize);
	b2=malloc(bsize);
	f = 0;
	
	ARGBEGIN{
	case 's':
		fmt=SUM;
		break;
	case 'h':
		fmt=HRS;
		break;
	case 'n':
		end=0;
		break;
	case 't':
		fmt=TM;
		break;
	case 'd':
		fmt=SECS;
		break;
	case 'l':
		fmt=LOCAL;
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
	Binit(&bin, f, OREAD);
	while(gline(b1) == 0) {
		prtime(b1);
	}
	if(end)
		prtime(0);
	return 0;
}

void prtime(char *b)
{
	char *f[3];
	static ulong now, last;
	static char name[256];
	static char rest[SIZE];
	Tm *tm;


	if(b){
		getfields(b, f, 3, 0, "\t\n");
		now = atol(f[0]);
	}else{
		now=time(0);
	}
	
	if(name[0] == '\0' && b){
		strcpy(name, f[1]);
		strcpy(rest, f[2]);
		last=now;
		return;
	}
	tm = localtime(last);

	switch(fmt){
	case SUM:
		print("%d\t%s\n",now-last,name);
		break;
	case HRS:
		print("%.2d:%.2d:%.2d   %s\t%s", tm->hour, tm->min, tm->sec, name, rest);
		break;
	case TM:
		print("%ld\t%s\t%ld\n", last, name, now-last);
		break;
	case LOCAL:
		print("%28.28s\t%s\t%s", asctime(tm), name, rest);
		break;
	case SECS:
	default:
		print("%ld\t%.4d/%.2d%.2d/%s\t%s",last,
		tm->year+1900,tm->mon+1,tm->mday, name,rest);
		break;
	}
	if(b){
		last=now;
		strcpy(name, f[1]);
		strcpy(rest, f[2]);
	}
}
