/*******************************************
 *	main(), parse arguments
 *******************************************/

#include "wily.h"
#include "tile.h"
 
static int	ncolumns = 2;
int	tagheight;
Tile	*wily=0;			/* encloses the whole application */

char *wilytools = 	"Kill | Newcol Quit Putall Dotfiles Font "; 
char *filetools = 	"Del Look .";
char *dirtools = 	"Del Look ..";
char *columntools = "Delcol New Cut Paste Snarf Anchor Split | ";

void
_allocerror(char*s){
	fprintf(stderr,"%s\n", s);
	abort();
}

/* Add  'cmd' to the visible list of running processes. */
void
addrunning(char *cmd){
	tag_addrunning(view_text(wily->tag), cmd);
}

/* Remove 'cmd' from the visible list of running processes.*/
void
rmrunning(char *cmd){
	tag_rmtool(view_text(wily->tag), cmd);
}

/* Initialize info about what bits of text get added to various tags.
 * Reads $WILYTOOLS or $HOME/.wilytools for regexp/tool pairs,
 * uses $WMAINTAG, $WCOLTAG $WFILETAG, $WDIRTAG 
 * as the tools for the wily tag, column tag, file tags and directory tags
 * respectively.
 */
static void
tools_init(void){
	char*s, t[200];
	
	if ((s = getenv("WILYTOOLS")))
		tag_init(s);
	else {
		Path p;
		sprintf(p, "%s/%s", getenv("HOME"), ".wilytools");
		tag_init(p);
	}
	
	if ((s=getenv("WCOLTAG")))
		columntools = strdup(s);
	if ((s=getenv("WMAINTAG"))) {
		strcpy(t, wilytools);
		strcat(t, s);
		wilytools = strdup(t);
	}
	if ((s=getenv("WFILETAG")))
		filetools = strdup(s);
	if ((s=getenv("WDIRTAG")))
		dirtools = strdup(s);
}

static void
usage(void) {
	fprintf(stderr, "wily [-c ncolumns] [-t tabsize] [-e command]\n");
	fprintf(stderr, "     [-f|-p9font varfont] [-F|-p9fixed fixfont] [file1 ...]\n");
	exit(1);
}

static void
args(int argc,char **argv, char **envp)
{
	int	i, c;
	char *cmd = 0;
	char *fixed = "fixed";

	frtabsize = 4;	/* default */

	/* filter out wily's options before letting X see them.
	 * otherwise -t gets interpreted as -title, for example.
	 * we need to leave X options and -p9font if it's there;
	 * libXg will pick that up.
	 */
	i = 1; 
	while (i < argc) {
		if (*argv[i] == '-') {
			int used = 0;

			/* accommodate previous wily usage */
			if (strcmp(argv[i], "-p9fixed") == 0)
				argv[i] = "-F";
			if (argv[i][2]) {
				++i;
				continue;
			}
			c = argv[i][1];
			switch (c) {
			case 'c':
				if (!argv[i+1])
					usage();
				ncolumns = atoi(argv[i+1]);
				if (ncolumns < 1)
					usage();
				used = 2;
				break;
			case 't':
				if (!argv[i+1])
					usage();
				frtabsize = atoi(argv[i+1]);
				if (frtabsize < 1)
					usage();
				used = 2;
				break;
			case 'e':
				if (!argv[i+1])
					usage();
				cmd = argv[i+1];
				used = 2;
				break;
			case 'f':
				/* rewrite acme option name for X */
				argv[i] = "-p9font";
				break;
			case 'F':
				if (!argv[i+1])
					usage();
				fixed = argv[i+1];
				used = 2;
				break;
			case 'h':
				usage();
				break;
			}
			if (used) {
				memmove(&argv[i], &argv[i+used], (argc-(i+used)+1)*sizeof(char *));
				argc -= used;
			} else
				++i;
			assert(argv[argc] == 0);
		} else
			++i;
	}

	/* init libXg */
	xtbinit((Errfunc)error, "Wily", &argc, argv, 0);
	tagheight = font->height + 2*INSET;

	initfont(fixed);
	scroll_init();
	einit(Ekeyboard | Emouse);
	cursorswitch(cursor);
	wily_init();

	ex_init();

	if (argc > 1) {
		for (i = 1; i < argc; ++i) {
			Path	label;

			data_addcontext(0, label, argv[i]);
			data_open(label, true);
		}
	} else if (cmd == 0) {
		data_open(wilydir, false);
	}
	if (cmd != 0)
		run(wily->tag, cmd, 0);
}

/* Initialise the base tile, with some columns */
void
wily_init(void)
{
	Text	*t;
	int	j;
	
	t = text_alloc(0, false);
	text_replaceutf(t, nr, wilytools);
	wily = tile_new(V, 0, screen.r.max.y, 0, 0, t, 0);
	tile_reshaped(wily);
	last_selection = 0;
	view_setlastselection( wily->tag);
	for(j=0; j< ncolumns; j++)
		col_new(wily->tag, 0);
}

/* Reshape the base tile */
void
ereshaped(Rectangle r)
{
	if(!wily)
		return;	/* not initialised yet */
	wily->cmax = r.max.x;
	wily->max = r.max.y;
	tile_reshaped(wily);
}

/* Main event loop */
static void
mainloop(void)
{
	ulong	type;
	Event	e;
	Bool	mouseaction=true;
	Point	lastp={0,0};
	View	*v=0;

	while ((type = eread(~0, &e))) {
		switch(type){
		case Ekeyboard:
			if(mouseaction) {
				/* 'v' is a cache of the view we are pointing at,
				 * (we use point to type).  We update this only
				 * when we start typing after moving the pointer.
				 */
				v = point2view(lastp);
				mouseaction=false;
			}
			if(v)
				dokeyboard(v, e.kbdc);
			break;

		case Emouse:
			lastp = e.mouse.xy;
			mouseaction=true;
			if(e.mouse.buttons && (v = point2view(e.mouse.xy)))
				domouse(v, &e.mouse);	
			break;

		default:
			dofd(type, e.n, (char*)e.data);
			break;
		}
	}
}

int
main(int argc,char **argv, char **envp)
{
#ifdef HAVE_GETCWD
	if (!getcwd(wilydir, sizeof(wilydir)-1))
		fatal("couldn't find out what directory this is: %s", wilydir);
#else
	if (!getwd(wilydir))
		fatal("couldn't find out what directory this is: %s", wilydir);
#endif
	add_slash(wilydir);
	env_init(envp);
	tools_init();
	args(argc,argv, envp);
	mainloop();
	return 0;
}
