#include <u.h>
#include <libc.h>
#include <msg.h>

#include <stdio.h>
/*#include <sys/time.h>*/
/*#include <sys/types.h>*/
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif /* HAVE_SYS_SELECT_H */
#include <assert.h>

/* Globals */
ulong 	op = 0;	/* output point - where we send output */
ulong	lastp = 0;	/* position of last character */
Handle	*handle;
Id		id;
char	*err;

enum {
	DEBUG = 0
};

#define DPRINT if(DEBUG)printf

void	connect_shell(char *const*);
void	handle_msg(void);
void	mainloop(void);
void error(char*);
static void	handle_shell(void);

int	wilyfd, fromshell, toshell;

void
main(int argc, char**argv)
{
	extern char *optarg;
	extern int optind;
	int	c;
	char label[256] ;

	sprintf(label, "+win-%d", getpid());
	while ((c = getopt(argc,	argv, "d")) != EOF)
		switch(c){
		case '?':
		default:
			fprintf(stderr, "%s [-d] [prog...]\n", argv[0]);
			exit(1);
		}
	if(DEBUG)
		setbuf(stdout,0);
	wilyfd = client_connect();
	handle = rpc_init(wilyfd);
	if( (err=rpc_new(handle, label, &id, 0)) ){
		fprintf(stderr, "rpc_new: %s\n", err);
		exit(1);
	}
	if( (err=rpc_attach(handle, id, WEexec | WEreplace)) ){
		fprintf(stderr, "rpc_attach: %s\n", err);
		exit(1);
	}

	connect_shell((optind<argc)?&argv[optind]:0);
	mainloop();
}

void
mainloop(void)
{
	fd_set	readfds;
	int		nready;

	for(;;) {
		DPRINT("op %lu\n", op);

		while(!rpc_wouldblock(handle))
			handle_msg();
			
		FD_ZERO(&readfds);
		FD_SET(wilyfd, &readfds);
		FD_SET(fromshell, &readfds);
		DPRINT("select\n");
		nready = select(MAX(wilyfd, fromshell)+1, &readfds, 0,0,0);
		if(nready<0)
			error("select");
		if (FD_ISSET(wilyfd, &readfds))
			handle_msg();
		else if (FD_ISSET(fromshell, &readfds))
			handle_shell();
		else
			fprintf(stderr, "wierd, man!\n");
	}
}

/* Spawn a shell connected to us with pipes.
 * PRE: $SHELL set to some shell, which should understand "-i" option
 */
void
connect_shell(char *const*argv)
{
        int     to[2], from[2];
        char	envbuf[256];

        if(pipe(to)||pipe(from)) {
                error("pipe");
        }

        switch(fork()) {
        case 0: /* shell */
                close(to[1]);
                close(from[0]);
                if(dup2(to[0], 0)<0 || dup2(from[1], 1)<0 || dup2(from[1], 2)<0)
                        error("dup2");
                        
		/* Let the shell know the window id */
		sprintf(envbuf, "WILYWIN=%d", id);
		putenv(envbuf);
		
		/* exec shell or command */
		if(argv==0) {
			char	*shell = getenv("SHELL");

			if(!shell)
				shell="/bin/sh";

			execl(shell, shell, "-i", 0);
		} else {
			execvp(argv[0],argv);
		}
		error("execl");
        case -1:
                error("fork");
                break;
        default:        /* parent */
                close(to[0]);
                close(from[1]);
                toshell = to[1];
                fromshell = from[0];
        }
}

static Bool shelloutput=false;

static void
handle_shell(void)
{
	char	buf[BUFSIZ];
	int		nread = read(fromshell,buf,BUFSIZ);

	if (nread>0) {
		buf [nread]='\0';
		if( (err=rpc_replace(handle, id, range(op, op), buf)) ){
			fprintf(stderr, "rpc_replace: %s\n",err);
			exit(1);
		}

		/* temporarily change op so shell output doesn't
		 * get sent back to the shell again
		 */
		op++;	
		shelloutput = true;
	} else
		exit(0);
}

void
handle_msg(void)
{
	int	len;
	char	*s;
	Msg	m;

	if(rpc_event(handle, &m)){
		fprintf(stderr,"lost connection");
		exit(1);
	}

	if (DEBUG) {
		printf("handle_msg: ");
		msg_print(&m);
	}

	switch(m.t){
	case WEdestroy:		/* window's gone.  shut down*/
		exit(0);
	case WEexec:
		/* Send m.s (perhaps with newline added) to lastp
		 */
		len = strlen(m.s);
		if(!len)
			break;
		if(m.s[len-1]=='\n')
			s = m.s;
		else {
			s = salloc(len + 2);
			sprintf(s, "%s\n", m.s);
		}
		rpc_replace(handle, id, range(op,op), s);
		if(s != m.s)
			free(s);
		break;
	case WEreplace:
		/* adjust op, lastp */ 
		DPRINT("replace: before op %lu , m.r (%lu %lu)\n", 
			op, m.r.p0, m.r.p1);

		op = ladjust(op, m.r, utflen(m.s));
		lastp = radjust(lastp, m.r, utflen(m.s));
		
		if (m.r.p1 >=op &&  strchr(m.s, '\n') ) {    /**/
			/* Send to the shell everything from op to the last nl,
			 * and adjust op.
			 */
			char	*lastnl = strrchr(m.s, '\n');
			int	len;
			char	*buf;
			ulong	endp;

			*lastnl = '\0';
			len = utflen(m.s);
			endp = m.r.p0 + len + 1;	/* +1 for the nl */
			buf = salloc(UTFmax * (endp - op));
			rpc_read(handle, id, range(op, endp), buf);
			len = strlen(buf);
			DPRINT ("sending to shell [%.*s]\n", len, buf);
			if(write(toshell, buf, len) != len)
				error("write to shell");
			free(buf);
			op = endp;
		}
		if(shelloutput){
			op--;
			shelloutput = false;
		}
		break;
	default:
		printf("unknown msg: ");
		msg_print(&m);
	}
}

void
error(char*msg)
{
	perror(msg);
	exit(1);
}

