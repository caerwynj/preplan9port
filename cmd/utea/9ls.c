#include "lib9.h"
#include <sys/stat.h>
#include <dirent.h>

typedef struct NDir NDir;
struct NDir
{
	Dir d;
	char	*prefix;
};

char	cwd[256];
int	errs = 0;
int	dflag;
int	eflag;
int	lflag;
int	nflag;
int	pflag;
int	qflag;
int	rflag;
int	sflag;
int	tflag;
int	uflag;
int	Fflag;
int	ndirbuf;
int	ndir;
NDir*	dirbuf;
int	ls(char*, int);
int	compar(NDir*, NDir*);
char*	asciitime(long);
char*	darwx(long);
void	rwx(long, char*);
void	growto(long);
void	dowidths(Dir*);
void	format(Dir*, char*);
void	output(void);
ulong	clk;
int	swidth;			/* max width of -s size */
int	qwidth;			/* max width of -q version */
int	vwidth;			/* max width of dev */
int	uwidth;			/* max width of userid */
int	glwidth;		/* max width of groupid and length */
Biobuf	bin;

#define		HUNK	50

void
main(int argc, char *argv[])
{
	int i, fd;
	char buf[64];
	char *name;

	Binit(&bin, 1, OWRITE);
	ARGBEGIN{
	case 'F':	Fflag++; break;
	case 'd':	dflag++; break;
	case 'e':	eflag++; break;
	case 'l':	lflag++; break;
	case 'n':	nflag++; break;
	case 'p':	pflag++; break;
	case 'q':	qflag++; break;
	case 'r':	rflag++; break;
	case 's':	sflag++; break;
	case 't':	tflag++; break;
	case 'u':	uflag++; break;
	default:	fprint(2, "usage: ls [-dlnpqrstuF] [file ...]\n");
			exits("usage");
	}ARGEND

	clk = time(0);
	getwd(cwd);
	if(argc == 0) {
		name = malloc(1+strlen(cwd)+1);
		if(name == nil) {
			fprint(2, "cleanname: out of memory\n");
			exits("out of memory");
		}
		sprint(name, "%s/.", cwd);
		cleanname(name);
		errs = ls(name, 1);
	}
	else for(i=0; i<argc; i++) {
		if(argv[i][0] == '/') {
			name = argv[i];
		} else {
			name = malloc(strlen(argv[i])+1+strlen(cwd)+1);
			if(name == nil) {
				fprint(2, "cleanname: out of memory\n");
				exits("out of memory");
			}
			sprint(name, "%s/%s", cwd, argv[i]);
			cleanname(name);
		}
		errs |= ls(name, 1);
	}
	output();
	exits(errs? "errors" : 0);
}

int
ls(char *s, int multi)
{
	DIR * fd;
	long i, n;
	char *p;
	Dir db;
	struct dirent * dp;
	char file[256];

	for(;;) {
		p = utfrrune(s, '/');
		if(p == 0 || p[1] != 0 || p == s)
			break;
		*p = 0;
	}
	if(dirstat(s, &db)!=0){
    error:
		fprint(2, "ls: %s: %r\n", s);
		return 1;
	}
	if(db.qid.path&CHDIR && dflag==0) {
		output();
		fd = opendir(s);
		if(fd == 0)
			goto error;
		while((dp = readdir(fd)) != 0) {
			sprint(file, "%s/%s", s, dp->d_name);
			dirstat(file, &db);
			growto(ndir+1);
			memmove(dirbuf+ndir, &db, sizeof(Dir));
			dirbuf[ndir].prefix = multi? s : 0;
			ndir++;
		}
		closedir(fd);
		output();
	}else{
		growto(ndir+1);
		memmove(dirbuf+ndir, &db, sizeof(Dir));
		dirbuf[ndir].prefix = 0;
		p = utfrrune(s, '/');
		if(p){
			dirbuf[ndir].prefix = s;
			*p = 0;
		}
		ndir++;
	}
	return 0;
}

void
output(void)
{
	int i;
	char buf[4096];
	char *s;

	if(!nflag)
		qsort(dirbuf, ndir, sizeof dirbuf[0], (int (*)(void*, void*))compar);
	for(i=0; i<ndir; i++)
		dowidths(&(dirbuf[i].d));
	for(i=0; i<ndir; i++) {
		if(!pflag && (s = dirbuf[i].prefix)) {
			if(strcmp(s, "/") ==0)	/* / is a special case */
				s = "";
			sprint(buf, "%s/%s", s, dirbuf[i].d.name);
			format(&(dirbuf[i].d), buf);
		} else
			format(&(dirbuf[i].d), dirbuf[i].d.name);
	}
	ndir = 0;
	Bflush(&bin);
}

void
dowidths(Dir *db)
{
	char buf[100];
	int n;

	if(sflag) {
		n = sprint(buf, "%llud", (db->length+1023)/1024);
		if(n > swidth)
			swidth = n;
	}
	if(qflag) {
		n = sprint(buf, "%lud", db->qid.vers);
		if(n > qwidth)
			qwidth = n;
	}
	if(lflag) {
		n = sprint(buf, "%ud", db->dev);
		if(n > vwidth)
			vwidth = n;
		n = strlen(db->uid);
		if(n > uwidth)
			uwidth = n;
		n = sprint(buf, "%d", db->length);
		n += strlen(db->gid);
		if(n > glwidth)
			glwidth = n;
	}
}

char*
fileflag(Dir *db)
{
	if(Fflag == 0)
		return "";
	if(CHDIR & db->qid.path)
		return "/";
	if(0111 & db->mode)
		return "*";
	return "";
}

void
format(Dir *db, char *name)
{
	if(sflag)
		Bprint(&bin, "%*d ",
			swidth, (db->length+1023)/1024);
	if(qflag)
		Bprint(&bin, "%.8x %*d ",
			db->qid.path,
			qwidth, db->qid.vers);
	if(lflag)
		Bprint(&bin, "%C %*d %*s %s %*d %s %s\n",
			db->type,
			vwidth, db->dev,
			-uwidth, db->uid,
			db->gid,
			(int)(glwidth-strlen(db->gid)), db->length,
			asciitime(uflag? db->atime : db->mtime), name);
	else
		Bprint(&bin, "%ld\t%s%s\t%ld\n", (uflag? db->atime: (eflag? clk : db->mtime)), name, fileflag(db), db->length);
}

void
growto(long n)
{
	if(n <= ndirbuf)
		return;
	ndirbuf = n;
	dirbuf=(NDir *)realloc(dirbuf, ndirbuf*sizeof(NDir));
	if(dirbuf == 0){
		fprint(2, "ls: malloc fail\n");
		exits("malloc fail");
	}		
}

int
compar(NDir *a, NDir *b)
{
	long i;

	if(tflag){
		if(uflag)
			i = b->d.atime-a->d.atime;
		else
			i = b->d.mtime-a->d.mtime;
	}else{
		if(a->prefix && b->prefix){
			i = strcmp(a->prefix, b->prefix);
			if(i == 0)
				i = strcmp(a->d.name, b->d.name);
		}else if(a->prefix){
			i = strcmp(a->prefix, b->d.name);
			if(i == 0)
				i = 1;	/* a is longer than b */
		}else if(b->prefix){
			i = strcmp(a->d.name, b->prefix);
			if(i == 0)
				i = -1;	/* b is longer than a */
		}else
			i = strcmp(a->d.name, b->d.name);
	}
	if(i == 0)
		i = (a<b? -1 : 1);
	if(rflag)
		i = -i;
	return i;
}

char*
asciitime(long l)
{
	static char buf[32];
	char *t;

	t = ctime(&l);
	/* 6 months in the past or a day in the future */
	if(l<clk-180L*24*60*60 || clk+24L*60*60<l){
		memmove(buf, t+4, 7);		/* month and day */
		memmove(buf+7, t+23, 5);		/* year */
	}else
		memmove(buf, t+4, 12);		/* skip day of week */
	buf[12] = 0;
	return buf;
}
