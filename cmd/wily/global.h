/* wily.c */
extern char*filetools; 
extern char*dirtools;

/* global.c */
extern Bool show_dot_files;
extern Bool autoindent_enabled;
extern View *last_selection;
extern char * notfilename, *notdoubleclick , *notcommand,
		*notaddress, *notinclude;
extern char*	whitespace;
extern Path	wilydir;

/* font.c */
extern FontUse *varfont, *fixedfont;

/* icons.c */
extern Cursor boxcursor;
extern Cursor fatarrow;
extern Cursor *cursor;

/* tile.c */
extern Tile *wily;

/* wily.c */
extern int tagheight;
