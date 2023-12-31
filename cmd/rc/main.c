/* main.c: handles initialization of rc and command line options */

#include "rc.h"

bool dashdee, dashee, dashvee, dashex, dashell, dasheye,
	dashen, dashpee, dashess, interactive;
pid_t rc_pid;

static bool dashoh;

static void assigndefault(char *,...);
static void checkfd(int, enum redirtype);

static const char id[] = "$Release: @(#)" PACKAGE " " VERSION " " RELDATE " $";

extern int main(int argc, char *argv[], char *envp[]) {
	char *dashsee[2], *dollarzero, *null[1];
	int c;
	initprint();
	dashsee[0] = dashsee[1] = NULL;
	dollarzero = argv[0];
	rc_pid = getpid();
	dashell = (*argv[0] == '-'); /* Unix tradition */
	while ((c = rc_getopt(argc, argv, "c:deilnopsVvx")) != -1)
		switch (c) {
		case 'c':
			dashsee[0] = rc_optarg;
			goto quitopts;
		case 'd':
			dashdee = TRUE;
			break;
		case 'e':
			dashee = TRUE;
			break;
		case 'i':
			dasheye = interactive = TRUE;
			break;
		case 'l':
			dashell = TRUE;
			break;
		case 'n':
			dashen = TRUE;
			break;
		case 'o':
			dashoh = TRUE;
			break;
		case 'p':
			dashpee = TRUE;
			break;
		case 's':
			dashess = TRUE;
			break;
		case 'V':
			fprint(1, "%s\n", id);
			exit(0);
		case 'v':
			dashvee = TRUE;
			break;
		case 'x':
			dashex = TRUE;
			break;
		case '?':
			exit(1);
		}
quitopts:
	argv += rc_optind;
	/* use isatty() iff -i is not set, and iff the input is not from a script or -c flags */
	if (!dasheye && dashsee[0] == NULL && (dashess || *argv == NULL))
		interactive = isatty(0);
	if (!dashoh) {
		checkfd(0, rFrom);
		checkfd(1, rCreate);
		checkfd(2, rCreate);
	}
	initsignal();
	inithash();
	initparse();
	assigndefault("prompt", "; ", "", (void *)0);
#ifdef DEFAULTPATH
	assigndefault("path", DEFAULTPATH, (void *)0);
#endif
	assigndefault("ifs", " ", "\t", "\n", (void *)0);
	assigndefault("pid", nprint("%d", rc_pid), (void *)0);
	initenv(envp);
	initinput();
	null[0] = NULL;
	starassign(dollarzero, null, FALSE); /* assign $0 to $* */
	inithandler();
	if (dashsee[0] != NULL || dashess) {	/* input from  -c or -s? */
		if (*argv != NULL)
			starassign(dollarzero, argv, FALSE);
		if (dashess)
			pushfd(0);
		else
			pushstring(dashsee, TRUE);
	} else if (*argv != NULL) {	/* else from a file? */
		b_dot(--argv);
		rc_exit(getstatus());
	} else {			/* else stdin */
		pushfd(0);
	}
	dasheye = FALSE;
	doit(TRUE);
	rc_exit(getstatus());
	return 0; /* Never really reached. */
}

static void assigndefault(char *name,...) {
	va_list ap;
	List *l;
	char *v;
	va_start(ap, name);
	for (l = NULL; (v = va_arg(ap, char *)) != NULL;)
		l = append(l, word(v, NULL));
	varassign(name, l, FALSE);
	if (streq(name, "path"))
		alias(name, l, FALSE);
	va_end(ap);
}

/* open an fd on /dev/null if it is inherited closed */

static void checkfd(int fd, enum redirtype r) {
	int new = rc_open("/dev/null", r);
	if (new != fd && new != -1)
		close(new);
}
