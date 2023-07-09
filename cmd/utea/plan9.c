#include	"lib9.h"
#include "fmt.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

/* for the ARGBEGIN macros */
char *argv0;

int
getfields(char *str, char **args, int max, int mflag, char *set)
{
	Rune r;
	int nr, intok, narg;

	if(max <= 0)
		return 0;

	narg = 0;
	args[narg] = str;
	if(!mflag)
		narg++;
	intok = 0;
	for(;; str += nr) {
		nr = chartorune(&r, str);
		if(r == 0)
			break;
		if(utfrune(set, r)) {
			if(narg >= max)
				break;
			*str = 0;
			intok = 0;
			args[narg] = str + nr;
			if(!mflag)
				narg++;
		} else {
			if(!intok && mflag)
				narg++;
			intok = 1;
		}
	}
	return narg;
}

/*
 * In place, rewrite name to compress multiple /, eliminate ., and process ..
 */
#define SEP(x)	((x)=='/' || (x) == 0)
char*
cleanname(char *name)
{
	char *p, *q, *dotdot;
	int rooted;

	rooted = name[0] == '/';

	/*
	 * invariants:
	 *	p points at beginning of path element we're considering.
	 *	q points just past the last path element we wrote (no slash).
	 *	dotdot points just past the point where .. cannot backtrack
	 *		any further (no slash).
	 */
	p = q = dotdot = name+rooted;
	while(*p) {
		if(p[0] == '/')	/* null element */
			p++;
		else if(p[0] == '.' && SEP(p[1]))
			p += 1;	/* don't count the separator in case it is nul */
		else if(p[0] == '.' && p[1] == '.' && SEP(p[2])) {
			p += 2;
			if(q > dotdot) {	/* can backtrack */
				while(--q > dotdot && *q != '/')
					;
			} else if(!rooted) {	/* /.. is / but ./../ is .. */
				if(q != name)
					*q++ = '/';
				*q++ = '.';
				*q++ = '.';
				dotdot = q;
			}
		} else {	/* real path element */
			if(q != name+rooted)
				*q++ = '/';
			while((*q = *p) != '/' && *q != 0)
				p++, q++;
		}
	}
	if(q == name)	/* empty string is really ``.'' */
		*q++ = '.';
	*q = '\0';
	return name;
}

static void
statconv(Dir *dir, struct stat *s)
{
	struct passwd *p;
	struct group *g;
	ulong q;

	p = getpwuid(s->st_uid);
	if (p)
		strncpy(dir->uid, p->pw_name, NAMELEN);
	g = getgrgid(s->st_gid);
	if (g)
		strncpy(dir->gid, g->gr_name, NAMELEN);
	q = 0;
	if(S_ISDIR(s->st_mode))
		q = CHDIR;
	q |= s->st_ino & 0x00FFFFFFUL;
	dir->qid.path = q;
	dir->qid.vers = s->st_mtime;
	dir->mode = (dir->qid.path&CHDIR)|(s->st_mode&0777);
	dir->atime = s->st_atime;
	dir->mtime = s->st_mtime;
	dir->length = s->st_size;
	dir->dev = s->st_dev;
	dir->type = 'M';
	if(S_ISFIFO(s->st_mode))
		dir->type = '|';
}

int
dirfstat(int fd, Dir *d)
{
	struct stat sbuf;

	if(fstat(fd, &sbuf) < 0)
		return -1;
	statconv(d, &sbuf);
	return 0;
}

char *
lelem(char *path)
{	
	char *pr;

	pr = utfrrune(path, '/');
	if(pr)
		pr++;
	else
		pr = path;
	return pr;
}

int
dirstat(char *f, Dir *d)
{
	struct stat sbuf;

	if(stat(f, &sbuf) < 0)
		return -1;
	statconv(d, &sbuf);
	strncpy(d->name, lelem(f), NAMELEN);
	return 0;
}

int
dirfwstat(int fd, Dir *d)
{
	return -1;
}

int
dirwstat(char *name, Dir *d)
{
	return -1;
}
