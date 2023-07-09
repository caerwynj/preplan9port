#define	DOMLEN		48
#define	DESKEYLEN	7
#define	CHALLEN		8
#define	TICKETLEN	(CHALLEN+2*NAMELEN+DESKEYLEN+1)
#define	AUTHENTLEN	(CHALLEN+4+1)

typedef	struct	Fcall	Fcall;

struct	Fcall
{
	char	type;
	short	fid;
	Ushort	tag;
	Ushort	oldtag;		/* T-Flush */
	Qid	qid;		/* R-Attach, R-Walk, R-Open, R-Create */
	char	client[NAMELEN];/* T-auth */
	char	chal[CHALLEN];	/* T-session, R-session */
	char	uname[NAMELEN];	/* T-Attach */
	char	aname[NAMELEN];	/* T-Attach */
	char	ticket[TICKETLEN]; /* T-Attach */
	char	auth[AUTHENTLEN]; /* T-Attach */
	char	rauth[AUTHENTLEN]; /* R-Attach */
	char	ename[ERRLEN];	/* R-Error */
	char	authid[NAMELEN]; /* R-session */
	char	authdom[DOMLEN]; /* R-session */
	long	perm;		/* T-Create */ 
	short	newfid;		/* T-Clone, T-Clwalk */
	char	name[NAMELEN];	/* T-Walk, T-Clwalk, T-Create */
	char	mode;		/* T-Create, T-Open */
	long	offset;		/* T-Read, T-Write */
	long	count;		/* T-Read, T-Write, R-Read */
	char	*data;		/* T-Write, R-Read */
	char	stat[DIRLEN];	/* T-Wstat, R-Stat */
};

#define	MAXFDATA	8192
#define	MAXMSG		128	/* max header sans data */
#define NOTAG		0xFFFF	/* Dummy tag */

enum
{
	Tnop =		50,
	Rnop,
	Terror =	54,	/* illegal */
	Rerror,
	Tflush =	56,
	Rflush,
	Tclone =	60,
	Rclone,
	Twalk =		62,
	Rwalk,
	Topen =		64,
	Ropen,
	Tcreate =	66,
	Rcreate,
	Tread =		68,
	Rread,
	Twrite =	70,
	Rwrite,
	Tclunk =	72,
	Rclunk,
	Tremove =	74,
	Rremove,
	Tstat =		76,
	Rstat,
	Twstat =	78,
	Rwstat,
	Tclwalk =	80,
	Rclwalk,
	Tsession =	84,
	Rsession,
	Tattach =	86,
	Rattach
};

int	convM2S(char*, Fcall*, int);
int	convS2M(Fcall*, char*);

int	convM2D(char*, Dir*);
int	convD2M(Dir*, char*);

int	fcallconv(void *, int, int, int, int);
int	dirconv(void *, int, int, int, int);

int	dirmodeconv(va_list*, Fconv*);
char*	getS(int, char*, Fcall*, long*);
