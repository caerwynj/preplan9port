#include <u.h>
#include <libc.h>
#include <bio.h>

#define	SIZE	1024
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
	exits("usage");
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

void
main(int argc, char * argv[]) {
	int  f;
	ulong  end=1;
	
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
			exits("open");
		}
	}
	Binit(&bin, f, OREAD);
	while(gline(b1) == 0) {
		if(b1[0] == '\0' || b1[0] == '\n') {
			prtime(0);
			print("\n");
		}
		prtime(b1);
	}
	if(end)
		prtime(0);
	exits(0);
}

void prtime(char *b)
{
	char *f[3];
	static ulong now, last;
	static char name[SIZE];
	static char rest[SIZE];
	Tm *tm;


	if(b && (b[0] != '\0') && (b[0] != '\n')){
		getfields(b, f, 3, 0, "\t\n");
		now = atol(f[0]);
	}else{
		now=time(0);
	}
	
	if(name[0] == '\0' && b && (b[0] != '\0') && (b[0] != '\n')){
		strcpy(name, f[1]);
		strcpy(rest, f[2]);
		last=now;
		return;
	} else if (name[0] == '\0' && !(b && (b[0] != '\0') && (b[0] != '\n'))){
		return;
	}
	tm = localtime(last);

	switch(fmt){
	case SUM:
		print("%ld\t%s\t%ld\n", last, name, now);
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
	if(b && (b[0] != '\0') && (b[0] != '\n')){
		last=now;
		strcpy(name, f[1]);
		strcpy(rest, f[2]);
	} else {
		name[0]='\0';
		rest[0]='\0';
	}
}
