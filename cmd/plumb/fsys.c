#include <u.h>
#include <libc.h>
#include <bio.h>
#include <regexp.h>
#include <thread.h>
#include <auth.h>
#include <fcall.h>
#include <plumb.h>
#include "plumber.h"

enum
{
	Stack = 8*1024
};

typedef struct Dirtab Dirtab;
typedef struct Fid Fid;
typedef struct Holdq Holdq;
typedef struct Readreq Readreq;
typedef struct Sendreq Sendreq;

struct Dirtab
{
	char		*name;
	uint		qid;
	uint		perm;
	int		nopen;		/* #fids open on this port */
	Fid		*fopen;
	Holdq	*holdq;
	Readreq	*readq;
	Sendreq	*sendq;
};

struct Fid
{
	int		fid;
	int		busy;
	int		open;
	int		mode;
	Qid		qid;
	Dirtab	*dir;
	long		offset;		/* zeroed at beginning of each message, read or write */
	char		*writebuf;		/* partial message written so far; offset tells how much */
	Fid		*next;
	Fid		*nextopen;
};

struct Readreq
{
	Fid		*fid;
	Fcall		*fcall;
	char		*buf;
	Readreq	*next;
};

struct Sendreq
{
	int			nfid;		/* number of fids that should receive this message */
	int			nleft;		/* number left that haven't received it */
	Fid			**fid;	/* fid[nfid] */
	Plumbmsg	*msg;
	char			*pack;	/* plumbpack()ed message */
	int			npack;	/* length of pack */
	Sendreq		*next;
};

struct Holdq
{
	Plumbmsg	*msg;
	Holdq		*next;
};

struct	/* needed because incref() doesn't return value */
{
	Lock;
	int			ref;
} rulesref;

enum
{
	DEBUG	= 0,
	NDIR	= 50,
	Nhash	= 16,

	Qdir		= 0,
	Qrules	= 1,
	Qsend	= 2,
	Qport	= 3,
	NQID	= Qport
};


static Dirtab dir[NDIR] =
{
	{ ".",			Qdir|CHDIR,	0500|CHDIR },
	{ "rules",		Qrules,		0600 },
	{ "send",		Qsend,		0200 },
};
static int	ndir = NQID;

static int		srvfd;
static int		clockfd;
static int		clock;
static Fid		*fids[Nhash];
static QLock	readlock;
static QLock	queue;
static char	srvfile[128];

static void	fsysproc(void*);
static void fsysrespond(Fcall*, char*, char*);
static Fid*	newfid(int);

static Fcall* fsysflush(Fcall*, char*, Fid*);
static Fcall* fsyssession(Fcall*, char*, Fid*);
static Fcall* fsysnop(Fcall*, char*, Fid*);
static Fcall* fsysattach(Fcall*, char*, Fid*);
static Fcall* fsysclone(Fcall*, char*, Fid*);
static Fcall* fsyswalk(Fcall*, char*, Fid*);
static Fcall* fsysopen(Fcall*, char*, Fid*);
static Fcall* fsyscreate(Fcall*, char*, Fid*);
static Fcall* fsysread(Fcall*, char*, Fid*);
static Fcall* fsyswrite(Fcall*, char*, Fid*);
static Fcall* fsysclunk(Fcall*, char*, Fid*);
static Fcall* fsysremove(Fcall*, char*, Fid*);
static Fcall* fsysstat(Fcall*, char*, Fid*);
static Fcall* fsyswstat(Fcall*, char*, Fid*);

Fcall* 	(*fcall[Tmax])(Fcall*, char*, Fid*) =
{
	[Tflush]	= fsysflush,
	[Tsession]	= fsyssession,
	[Tnop]	= fsysnop,
	[Tattach]	= fsysattach,
	[Tclone]	= fsysclone,
	[Twalk]	= fsyswalk,
	[Topen]	= fsysopen,
	[Tcreate]	= fsyscreate,
	[Tread]	= fsysread,
	[Twrite]	= fsyswrite,
	[Tclunk]	= fsysclunk,
	[Tremove]= fsysremove,
	[Tstat]	= fsysstat,
	[Twstat]	= fsyswstat,
};

char	Ebadfcall[] =	"bad fcall type";
char	Eperm[] = 	"permission denied";
char	Eoffset[] =		"bad offset in directory";
char	Enomem[] =	"malloc failed for buffer";
char	Enotdir[] =	"not a directory";
char	Enoexist[] =	"plumb file does not exist";
char	Eisdir[] =		"file is a directory";
char	Ebadmsg[] =	"bad plumb message format";
char Enosuchport[] ="no such plumb port";
char Enoport[] =	"couldn't find destination for message";
char	Einuse[] = 	"file already open";

/*
 * Add new port.  A no-op if port already exists or is the null string
 */
void
addport(char *port)
{
	int i;
	void **a;

	if(port == nil)
		return;
	for(i=NQID; i<ndir; i++)
		if(strcmp(port, dir[i].name) == 0)
			return;
	if(i == NDIR){
		fprint(2, "plumb: too many ports; max %d\n", NDIR);
		return;
	}
	ndir++;
	dir[i].name = estrdup(port);
	dir[i].qid = i;
	dir[i].perm = 0400;
	a = emalloc(sizeof(void**));
	a[0] = &dir[i];
	nports++;
	ports = erealloc(ports, nports*sizeof(char*));
	ports[nports-1] = dir[i].name;
}

static ulong
getclock(void)
{
	char buf[32];

	seek(clockfd, 0, 0);
	read(clockfd, buf, sizeof buf);
	return atoi(buf);
}

void
startfsys(void*)
{
	int p[2], fd;

	fmtinstall('F', fcallconv);
	clockfd = open("/dev/time", OREAD|OCEXEC);
	clock = getclock();
	if(pipe(p) < 0)
		error("can't create pipe: %r");
	/* 0 will be server end, 1 will be client end */
	srvfd = p[0];
	sprint(srvfile, "/srv/plumb.%s.%d", user, getpid());
	if(putenv("plumbsrv", srvfile) < 0)
		error("can't write $plumbsrv: %r");
	fd = create(srvfile, OWRITE|OCEXEC|ORCLOSE, 0600);
	if(fd < 0)
		error("can't create /srv file: %r");
	if(threadprint(fd, "%d", p[1]) <= 0)
		error("can't write /srv/file: %r");
	/* leave fd open; ORCLOSE will take care of it */

	proccreate(fsysproc, nil, Stack);

	rfork(RFFDG);
	close(p[0]);
	if(mount(p[1], "/mnt/plumb", MREPL, "") < 0)
		error("can't mount /mnt/plumb: %r");
	threadhack = 0;
	close(p[1]);
}

static void
fsysproc(void*)
{
	int n;
	Fcall *t;
	Fid *f;
	char *buf;

	t = nil;
	for(;;){
		buf = malloc(MAXFDATA+MAXMSG);	/* avoid memset of emalloc */
		if(buf == nil)
			error("malloc failed: %r");
		qlock(&readlock);
		n = read(srvfd, buf, MAXFDATA+MAXMSG);
		if(n <= 0)
			error("i/o error on server channel");
		if(readlock.head == nil)	/* no other processes waiting to read; start one */
			proccreate(fsysproc, nil, Stack);
		qunlock(&readlock);
		if(t == nil)
			t = emalloc(sizeof(Fcall));
		if(convM2S(buf, t, n) != n)
			error("convert error in convM2S");
		if(DEBUG)
			threadprint(2, "%F\n", t);
		if(fcall[t->type] == nil)
			fsysrespond(t, buf, Ebadfcall);
		else{
			if(t->type==Tnop || t->type==Tsession)
				f = nil;
			else
				f = newfid(t->fid);
			t = (*fcall[t->type])(t, buf, f);
		}
	}
}

static void
fsysrespond(Fcall *t, char *buf, char *err)
{
	int n;

	if(err){
		t->type = Rerror;
		strncpy(t->ename, err, ERRLEN);
	}else
		t->type++;
	if(buf == nil)
		buf = emalloc(MAXFDATA+MAXMSG);
	n = convS2M(t, buf);
	if(n < 0)
		error("convert error in convS2M");
	if(write(srvfd, buf, n) != n)
		error("write error in respond");
	if(DEBUG)
		threadprint(2, "%F\n", t);
	free(buf);
}

static
Fid*
newfid(int fid)
{
	Fid *f, *ff, **fh;

	qlock(&queue);
	ff = nil;
	fh = &fids[fid&(Nhash-1)];
	for(f=*fh; f; f=f->next)
		if(f->fid == fid)
			goto Return;
		else if(ff==nil && !f->busy)
			ff = f;
	if(ff){
		ff->fid = fid;
		f = ff;
		goto Return;
	}
	f = emalloc(sizeof *f);
	f->fid = fid;
	f->next = *fh;
	*fh = f;
    Return:
	qunlock(&queue);
	return f;
}

static void
dostat(Dirtab *dir, char *buf, uint clock)
{
	Dir d;

	d.qid.path = dir->qid;
	d.qid.vers = 0;
	d.mode = dir->perm;
	d.length = 0;	/* would be nice to do better */
	strncpy(d.name, dir->name, NAMELEN);
	memmove(d.uid, user, NAMELEN);
	memmove(d.gid,user, NAMELEN);
	d.atime = clock;
	d.mtime = clock;
	convD2M(&d, buf);
}

static void
queuesend(Dirtab *d, Plumbmsg *m)
{
	Sendreq *s, *t;
	Fid *f;
	int i;

	s = emalloc(sizeof(Sendreq));
	s->nfid = d->nopen;
	s->nleft = s->nfid;
	s->fid = emalloc(s->nfid*sizeof(Fid*));
	i = 0;
	/* build array of fids open on this channel */
	for(f=d->fopen; f!=nil; f=f->nextopen)
		s->fid[i++] = f;
	s->msg = m;
	s->next = nil;
	/* link to end of queue; drainqueue() searches in sender order so this implements a FIFO */
	for(t=d->sendq; t!=nil; t=t->next)
		if(t->next == nil)
			break;
	if(t == nil)
		d->sendq = s;
	else
		t->next = s;
}

static void
queueread(Dirtab *d, Fcall *t, char *buf, Fid *f)
{
	Readreq *r;

	r = emalloc(sizeof(Readreq));
	r->fcall = t;
	r->buf = buf;
	r->fid = f;
	r->next = d->readq;
	d->readq = r;
}

static void
drainqueue(Dirtab *d)
{
	Readreq *r, *nextr, *prevr;
	Sendreq *s, *nexts, *prevs;
	int i, n;

	prevs = nil;
	for(s=d->sendq; s!=nil; s=nexts){
		nexts = s->next;
		for(i=0; i<s->nfid; i++){
			prevr = nil;
			for(r=d->readq; r!=nil; r=nextr){
				nextr = r->next;
				if(r->fid == s->fid[i]){
					/* pack the message if necessary */
					if(s->pack == nil)
						s->pack = plumbpack(s->msg, &s->npack);
					/* exchange the stuff... */
					r->fcall->data = s->pack+r->fid->offset;
					n = s->npack - r->fid->offset;
					if(n > MAXFDATA)
						n = MAXFDATA;
					if(n > r->fcall->count)
						n = r->fcall->count;
					r->fcall->count = n;
					fsysrespond(r->fcall, r->buf, nil);
					r->fid->offset += n;
					if(r->fid->offset >= s->npack){
						/* message transferred; delete this fid from send queue */
						r->fid->offset = 0;
						s->fid[i] = nil;
						s->nleft--;
					}
					/* delete read request from queue */
					if(prevr)
						prevr->next = r->next;
					else
						d->readq = r->next;
					free(r->fcall);
					free(r);
					break;
				}else
					prevr = r;
			}
		}
		/* if no fids left, delete this send from queue */
		if(s->nleft == 0){
			free(s->fid);
			plumbfree(s->msg);
			free(s->pack);
			if(prevs)
				prevs->next = s->next;
			else
				d->sendq = s->next;
			free(s);
		}else
			prevs = s;
	}
}

/* can't flush a send because they are always answered synchronously */
static void
flushqueue(Dirtab *d, int oldtag)
{
	Readreq *r, *prevr;

	prevr = nil;
	for(r=d->readq; r!=nil; r=r->next){
		if(oldtag == r->fcall->tag){
			/* delete read request from queue */
			if(prevr)
				prevr->next = r->next;
			else
				d->readq = r->next;
			free(r->fcall);
			free(r);
			return;
		}
		prevr = r;
	}
}

/* remove messages awaiting delivery to now-closing fid */
static void
removesenders(Dirtab *d, Fid *fid)
{
	Sendreq *s, *nexts, *prevs;
	int i;

	prevs = nil;
	for(s=d->sendq; s!=nil; s=nexts){
		nexts = s->next;
		for(i=0; i<s->nfid; i++)
			if(fid == s->fid[i]){
				/* delete this fid from send queue */
				s->fid[i] = nil;
				s->nleft--;
				break;
			}
		/* if no fids left, delete this send from queue */
		if(s->nleft == 0){
			free(s->fid);
			plumbfree(s->msg);
			free(s->pack);
			if(prevs)
				prevs->next = s->next;
			else
				d->sendq = s->next;
			free(s);
		}else
			prevs = s;
	}
}

static void
hold(Plumbmsg *m, Dirtab *d)
{
	Holdq *h, *q;

	h = emalloc(sizeof(Holdq));
	h->msg = m;
	/* add to end of queue */
	if(d->holdq == nil)
		d->holdq = h;
	else{
		for(q=d->holdq; q->next!=nil; q=q->next)
			;
		q->next = h;
	}
}

static void
queueheld(Dirtab *d)
{
	Holdq *h;

	while(d->holdq != nil){
		h = d->holdq;
		d->holdq = h->next;
		queuesend(d, h->msg);
		/* no need to drain queue because we know no-one is reading yet */
		free(h);
	}
}

static void
dispose(Fcall *t, char *buf, Plumbmsg *m, Ruleset *rs, Exec *e)
{
	int i;
	char *err;

	qlock(&queue);
	err = nil;
	if(m->dst==nil || m->dst[0]=='\0'){
		err = Enoport;
		if(rs != nil)
			err = startup(rs, e);
		plumbfree(m);
	}else
		for(i=NQID; i<ndir; i++)
			if(strcmp(m->dst, dir[i].name) == 0){
				if(dir[i].nopen == 0){
					err = startup(rs, e);
					if(e!=nil && e->holdforclient)
						hold(m, &dir[i]);
					else
						plumbfree(m);
				}else{
					queuesend(&dir[i], m);
					drainqueue(&dir[i]);
				}
				break;
			}
	freeexec(e);
	qunlock(&queue);
	fsysrespond(t, buf, err);
	free(t);
}

static Fcall*
fsysnop(Fcall *t, char *buf, Fid*)
{
	fsysrespond(t, buf, nil);
	return t;
}

static Fcall*
fsyssession(Fcall *t, char *buf, Fid*)
{
	Fcall out;

	memset(&out, 0, sizeof(Fcall));
	out.type = t->type;
	out.tag = t->tag;
	fsysrespond(&out, buf, nil);
	return t;
}

static Fcall*
fsysattach(Fcall *t, char *buf, Fid *f)
{
	Fcall out;

	if(strcmp(t->uname, user) != 0){
		fsysrespond(&out, buf, Eperm);
		return t;
	}
	f->busy = 1;
	f->open = 0;
	f->qid = (Qid){CHDIR|Qdir, 0};
	f->dir = dir;
	memset(&out, 0, sizeof(Fcall));
	out.type = t->type;
	out.tag = t->tag;
	out.fid = f->fid;
	out.qid = f->qid;
	fsysrespond(&out, buf, nil);
	return t;
}

static Fcall*
fsysflush(Fcall *t, char *buf, Fid*)
{
	int i;

	qlock(&queue);
	for(i=NQID; i<ndir; i++)
		flushqueue(&dir[i], t->oldtag);
	qunlock(&queue);
	fsysrespond(t, buf, nil);
	return t;
}

static Fcall*
fsysclone(Fcall *t, char *buf, Fid *f)
{
	Fid *nf;

	if(f->open){
		fsysrespond(t, buf, "is open");
		return t;
	}
	nf = newfid(t->newfid);
	nf->busy = 1;
	nf->open = 0;
	nf->dir = f->dir;
	nf->qid = f->qid;

	fsysrespond(t, buf, nil);
	return t;
}

static Fcall*
fsyswalk(Fcall *t, char *buf, Fid *f)
{
	ulong qid;
	Dirtab *d;

	if((f->qid.path & CHDIR) == 0){
		fsysrespond(t, buf, Enotdir);
		return t;
	}
	if(strcmp(t->name, "..") == 0){
		qid = Qdir|CHDIR;
		goto Found;
	}
	d = dir;
	d++;	/* skip '.' */
	for(; d->name; d++)
		if(strcmp(t->name, d->name) == 0){
			qid = d->qid;
			f->dir = d;
			goto Found;
		}

	fsysrespond(t, buf, Enoexist);
	return t;

    Found:
	f->qid = (Qid){qid, 0};
	t->qid = f->qid;
	fsysrespond(t, buf, nil);
	return t;
}

static Fcall*
fsysopen(Fcall *t, char *buf, Fid *f)
{
	int m, clearrules;

	clearrules = 0;
	if(t->mode & OTRUNC){
		if(f->qid.path != Qrules)
			goto Deny;
		clearrules = 1;
	}
	/* can't truncate anything, so just disregard */
	t->mode &= ~(OTRUNC|OCEXEC);
	/* can't execute or remove anything */
	if(t->mode==OEXEC || (t->mode&ORCLOSE))
		goto Deny;
	switch(t->mode){
	default:
		goto Deny;
	case OREAD:
		m = 0400;
		break;
	case OWRITE:
		m = 0200;
		break;
	case ORDWR:
		m = 0600;
		break;
	}
	if(((f->dir->perm&~(CHDIR|CHAPPEND))&m) != m)
		goto Deny;
	if(f->qid.path==Qrules && (t->mode==OWRITE || t->mode==ORDWR)){
		lock(&rulesref);
		if(rulesref.ref++ != 0){
			rulesref.ref--;
			unlock(&rulesref);
			fsysrespond(t, buf, Einuse);
			return t;
		}
		unlock(&rulesref);
	}
	if(clearrules){
		writerules(nil, 0);
		rules[0] = nil;
	}
	t->qid = f->qid;
	qlock(&queue);
	f->mode = t->mode;
	f->open = 1;
	f->dir->nopen++;
	f->nextopen = f->dir->fopen;
	f->dir->fopen = f;
	queueheld(f->dir);
	qunlock(&queue);
	fsysrespond(t, buf, nil);
	return t;

    Deny:
	fsysrespond(t, buf, Eperm);
	return t;
}

static Fcall*
fsyscreate(Fcall *t, char *buf, Fid*)
{
	fsysrespond(t, buf, Eperm);
	return t;
}

static Fcall*
fsysreadrules(Fcall *t, char *buf)
{
	char *p;
	int n;

	p = printrules();
	n = strlen(p);
	t->data = p;
	if(t->offset >= n)
		t->count = 0;
	else{
		t->data = p+t->offset;
		if(t->offset+t->count > n)
			t->count = n-t->offset;
	}
	fsysrespond(t, buf, nil);
	free(p);
	return t;
}

static Fcall*
fsysread(Fcall *t, char *buf, Fid *f)
{
	char *b;
	int i, n, o, e;
	Dirtab *d;

	if(f->qid.path != (Qdir|CHDIR)){
		if(f->qid.path == Qrules)
			return fsysreadrules(t, buf);
		/* read from port */
		if(f->qid.path < NQID){
			fsysrespond(t, buf, "internal error: unknown read port");
			return t;
		}
		qlock(&queue);
		queueread(f->dir, t, buf, f);
		drainqueue(f->dir);
		qunlock(&queue);
		return nil;
	}
	if(t->offset % DIRLEN){
		fsysrespond(t, buf, Eoffset);
		return t;
	}
	o = t->offset;
	e = t->offset+t->count;
	b = malloc(MAXFDATA+MAXMSG);
	if(b == nil){
		fsysrespond(t, buf, Enomem);
		return t;
	}
	n = 0;
	d = dir;
	d++;	/* first entry is '.' */
	for(i=0; d->name!=nil && i+DIRLEN<e; i+=DIRLEN){
		if(i >= o){
			dostat(d, b+n, clock);
			n += DIRLEN;
		}
		d++;
	}
	t->data = b;
	t->count = n;
	fsysrespond(t, buf, nil);
	free(b);
	return t;
}

static Fcall*
fsyswrite(Fcall *t, char *buf, Fid *f)
{
	Plumbmsg *m;
	int i, n;
	long count;
	char *data;
	Exec *e;

	switch(f->qid.path){
	case Qdir|CHDIR:
		fsysrespond(t, buf, Eisdir);
		return t;
	case Qrules:
		clock = getclock();
		fsysrespond(t, buf, writerules(t->data, t->count));
		return t;
	case Qsend:
		if(f->offset == 0){
			data = t->data;
			count = t->count;
		}else{
			/* partial message already assembled */
			f->writebuf = erealloc(f->writebuf, f->offset + t->count);
			memmove(f->writebuf+f->offset, t->data, t->count);
			data = f->writebuf;
			count = f->offset+t->count;
		}
		m = plumbunpackpartial(data, count, &n);
		if(m == nil){
			if(n == 0){
				f->offset = 0;
				free(f->writebuf);
				f->writebuf = nil;
				fsysrespond(t, buf, Ebadmsg);
				return t;
			}
			/* can read more... */
			if(f->offset == 0){
				f->writebuf = emalloc(t->count);
				memmove(f->writebuf, t->data, t->count);
			}
			/* else buffer has already been grown */
			f->offset += t->count;
			fsysrespond(t, buf, nil);
			return t;
		}
		/* release partial buffer */
		f->offset = 0;
		free(f->writebuf);
		f->writebuf = nil;
		for(i=0; rules[i]; i++)
			if((e=matchruleset(m, rules[i])) != nil){
				dispose(t, buf, m, rules[i], e);
				return nil;
			}
		if(m->dst != nil){
			dispose(t, buf, m, nil, nil);
			return nil;
		}
		fsysrespond(t, buf, "no matching plumb rule");
		return t;
	}
	fsysrespond(t, buf, "internal error: write to unknown file");
	return t;
}

static Fcall*
fsysstat(Fcall *t, char *buf, Fid *f)
{
	dostat(f->dir, t->stat, clock);
	fsysrespond(t, buf, nil);
	return t;
}

static Fcall*
fsyswstat(Fcall *t, char *buf, Fid*)
{
	fsysrespond(t, buf, Eperm);
	return t;
}

static Fcall*
fsysremove(Fcall *t, char *buf, Fid*)
{
	fsysrespond(t, buf, Eperm);
	return t;
}

static Fcall*
fsysclunk(Fcall *t, char *buf, Fid *f)
{
	Fid *prev, *p;
	Dirtab *d;

	qlock(&queue);
	if(f->open){
		d = f->dir;
		d->nopen--;
		if(d->qid==Qrules && (f->mode==OWRITE || f->mode==ORDWR)){
			/*
			 * just to be sure last rule is parsed; error messages will be lost, though,
			 * unless last write ended with a blank line
			 */
			writerules(nil, 0);
			lock(&rulesref);
			rulesref.ref--;
			unlock(&rulesref);
		}
		prev = nil;
		for(p=d->fopen; p; p=p->nextopen){
			if(p == f){
				if(prev)
					prev->nextopen = f->nextopen;
				else
					d->fopen = f->nextopen;
				removesenders(d, f);
				break;
			}
			prev = p;
		}
	}
	f->busy = 0;
	f->open = 0;
	f->offset = 0;
	if(f->writebuf != nil){
		free(f->writebuf);
		f->writebuf = nil;
	}
	qunlock(&queue);
	fsysrespond(t, buf, nil);
	return t;
}
