#include "u.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "libc.h"

char*	bldpath(char*, char*, char*);
Ulong	vers(struct stat*);
Ulong	qid(struct stat*);
char*	id2name(int);
char*   lelem(char *path);
int
dirstat(char *name, Dir *dir)
{
	Dir d;
	struct stat stbuf;

	if(stat(name, &stbuf) < 0)
		return 1;
	strncpy(d.name, lelem(name), NAMELEN);
	strncpy(d.uid, id2name(stbuf.st_uid), NAMELEN);
	strncpy(d.gid, id2name(stbuf.st_gid), NAMELEN);
	d.qid.path = qid(&stbuf);
	d.qid.vers = vers(&stbuf);
	d.mode = ((d.qid.path&CHDIR)|(stbuf.st_mode&0777));
	d.atime = stbuf.st_atime;
	d.mtime = stbuf.st_mtime;
	d.len.hlength = 0;
	d.len.length = stbuf.st_size;
	d.type=0;
	d.dev=0;
	*dir=d;
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

/*
 * qids: directory bit, seven bits of device, 24 bits of inode
 */
Ulong
vers(struct stat *st)
{
	return st->st_mtime ^ (st->st_size<<8);
}

Ulong
qid(struct stat *st)
{
	Ulong q;

	q = 0;
	if((st->st_mode&S_IFMT) ==  S_IFDIR)
		q = CHDIR;
	q |= 1<<24;
	q |= st->st_ino & 0x00FFFFFFUL;
	return q;
}
char*
id2name(int id)
{
	char buf[40];

	sprintf(buf, "%d", id);
	return buf;
}
