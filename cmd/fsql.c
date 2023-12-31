/* This program accepts arbitrary SQL statements from the user,
   and processes the statement.  Statements may be entered on
   multiple lines, and must be terminated by a semi-colon.
   If a query, the results are printed. 

   To quit the program, type EXIT.

   The size of the HDA is defined by the HDA_SIZE constant,
   which is declared in ocidem.h to be 256 bytes for 32-
   bit architectures and 512 bytes for 64-bit architectures.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>


/* Include OCI-specific headers. */
#include <oratypes.h>
#include <ocidfn.h>
#ifdef __STDC__
#include <ociapr.h>
#else
#include <ocikpr.h>
#endif
#include <ocidem.h>


/* Constants used in this program. */
#define MAX_BINDS               12
#define MAX_ITEM_BUFFER_SIZE    256
#define MAX_SELECT_LIST_SIZE    12
#define MAX_SQL_IDENTIFIER      31
#define OCI_EXIT_FAILURE 1
#define OCI_EXIT_SUCCESS 0

#define PARSE_NO_DEFER           0
#define PARSE_V7_LNG             2
#define LONG_SIZE 		256

/* Define one logon data area and one cursor data area
   Also define a host data area for olog.
   (See ocidfn.h for declarations). */
Lda_Def lda;
Cda_Def cda;
ub4 hda[HDA_SIZE / (sizeof(ub4))];

/* Declare an array of bind values. */
text bind_values[MAX_BINDS][MAX_ITEM_BUFFER_SIZE];

/* Declare structures for query information. */
struct describe {
	sb4 dbsize;
	sb2 dbtype;
	sb1 buf[MAX_ITEM_BUFFER_SIZE];
	sb4 buflen;
	sb4 dsize;
	sb2 precision;
	sb2 scale;
	sb2 nullok;
};

struct define {
	ub1 buf[MAX_ITEM_BUFFER_SIZE];
	float flt_buf;
	sword int_buf;
	sb2 indp;
	ub2 col_retlen, col_retcode;
};


/* Define arrays of describe and define structs. */
struct describe desc[MAX_SELECT_LIST_SIZE];
struct define def[MAX_SELECT_LIST_SIZE];

/*  Declare this programs functions. */
sword connect_user(unsigned char * username, unsigned char * password);
sword describe_define();
sword do_binds();
void do_exit();
void oci_error();
sword get_sql_statement();
void print_header();
void print_rows();
void fetch_long(Cda_Def * cda, sword ncols);
void err_report(Cda_Def * cursor);

/* Globals */
static text sql_statement[2048];
static sword sql_function;
static sword numwidth = 8;
#define	ARGBEGIN	for((argv0? 0: (argv0= *argv)),argv++,argc--;\
			    argv[0] && argv[0][0]=='-' && argv[0][1];\
			    argc--, argv++) {\
				char *_args, *_argt, _argc;\
				_args = &argv[0][1];\
				if(_args[0]=='-' && _args[1]==0){\
					argc--; argv++; break;\
				}\
				_argc=0;while(*_args) switch(_argc= *_args++)
#define	ARGEND		}
#define	ARGF()		(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	EARGF(x)	(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): ((x), exit(1), (char*)0)))

#define	ARGC()		_argc
char *argv0;

void
usage(void)
{
	fprintf(stderr, "usage: fsql [-q] [-h] [-t delim]  user  passwd\n");
	exit(-1);
}
int quote, head;
char * delim = NULL;
char * rs = '\n';
main(int argc, char * argv[])
{
	sword col, errno, n, ncols;
	text *cp;

	ARGBEGIN{
	case 'q':
		quote=1;
		break;
	case 'h':
		head=1;
		break;
	case 't':
		delim=EARGF(usage());
		break;
	case 'r':
		rs=ARGF()[0];
	default:
		usage();
	}ARGEND	
	if (argc != 2) {
		usage();
	}
	if(!delim)
		delim=strdup(",");
	/* Connect to ORACLE. */
	if (connect_user((unsigned char *) argv[0],(unsigned char *)  argv[1]))
		exit(-1);

	/* Open a cursor, exit on error (unrecoverable). */
	if (oopen(&cda, &lda, (text *) 0, -1, -1, (text *) 0, -1)) {
		printf("Error opening cursor.  Exiting...\n");
		ologof(&lda);
		exit(-1);
	}

	/* Process user's SQL statements. */

	for (;;) {
		/* Get the statement, exit on "exit". */
		if (get_sql_statement())
			do_exit(0);

		/* Parse the statement; do not defer the parse,
		   so that errors come back right away. */
		if (oparse(&cda, (text *) sql_statement, (sb4) - 1,
			   (sword) PARSE_NO_DEFER, (ub4) PARSE_V7_LNG)) {
			oci_error(&cda);
			continue;
		}

		/* Save the SQL function code right after parse. */
		sql_function = cda.ft;

		/* Bind any input variables. */
		if ((ncols = do_binds(&cda, sql_statement)) == -1)
			continue;

		/* If the statement is a query, describe and define
		   all select-list items before doing the oexec. */
		if (sql_function == FT_SELECT)
			if ((ncols = describe_define(&cda)) == -1)
				continue;

		/* Execute the statement. */
		if (oexec(&cda)) {
			oci_error(&cda);
			continue;
		}

		/* Fetch and display the rows for the query. */
		if (sql_function == FT_SELECT) {
			if(head)
				print_header(ncols);
			print_rows(&cda, ncols);
		}
	}			/* end for (;;) */
}				/* end main() */


sword connect_user(unsigned char * username, unsigned char * password)
{
	sword n;

	if (olog(&lda, (ub1 *) hda, username, -1, password, -1,
		 (text *) 0, -1, (ub4) OCI_LM_DEF)) {
		printf("Cannot connect as %s.\n", username);
		printf("Try again.\n\n");
	} else {
		return 0;
	}
	printf("Connection failed.  Exiting...\n");
	return -1;
}


/*  Describe select-list items. */

sword describe_define(cda)
Cda_Def *cda;
{
	sword col, deflen, deftyp;
	static ub1 *defptr;

	/* Describe the select-list items. */
	for (col = 0; col < MAX_SELECT_LIST_SIZE; col++) {
		desc[col].buflen = MAX_ITEM_BUFFER_SIZE;
		if (odescr(cda, col + 1, &desc[col].dbsize,
			   &desc[col].dbtype, &desc[col].buf[0],
			   &desc[col].buflen, &desc[col].dsize,
			   &desc[col].precision, &desc[col].scale,
			   &desc[col].nullok)) {
			/* Break on end of select list. */
			if (cda->rc == VAR_NOT_IN_LIST)
				break;
			else {
				oci_error(cda);
				return -1;
			}
		}
		/* adjust sizes and types for display */
		switch (desc[col].dbtype) {
		case NUMBER_TYPE:
			desc[col].dbsize = numwidth;
			/* Handle NUMBER with scale as float. */
			if (desc[col].scale != 0) {
				defptr = (ub1 *) & def[col].flt_buf;
				deflen = (sword) sizeof(float);
				deftyp = FLOAT_TYPE;
				desc[col].dbtype = FLOAT_TYPE;
			} else {
				defptr = (ub1 *) & def[col].int_buf;
				deflen = (sword) sizeof(sword);
				deftyp = INT_TYPE;
				desc[col].dbtype = INT_TYPE;
			}
			break;
		case SQLT_LNG:
			defptr = def[col].buf;
			deflen = MAX_ITEM_BUFFER_SIZE - 1;
			deftyp = SQLT_LNG;
			break;
		default:
			if (desc[col].dbtype == DATE_TYPE)
				desc[col].dbsize = 9;
			if (desc[col].dbtype == ROWID_TYPE)
				desc[col].dbsize = 18;
			defptr = def[col].buf;
			deflen = desc[col].dbsize > MAX_ITEM_BUFFER_SIZE ?
			    MAX_ITEM_BUFFER_SIZE : desc[col].dbsize + 1;
			deftyp = STRING_TYPE;
			break;
		}
		if (odefin(cda, col + 1,
			   defptr, deflen, deftyp,
			   -1, &def[col].indp, (text *) 0, -1, -1,
			   &def[col].col_retlen, &def[col].col_retcode)) {
			oci_error(cda);
			return -1;
		}
	}
	return col;
}


/*  Bind input variables. */

sword do_binds(cda, stmt_buf)
Cda_Def *cda;
text *stmt_buf;
{
	sword i, in_literal, n;
	text *cp, *ph;

	/* Find and bind input variables for placeholders. */
	for (i = 0, in_literal = FALSE, cp = stmt_buf;
	     *cp && i < MAX_BINDS; cp++) {
		if (*cp == '\'')
			in_literal = ~in_literal;
		if (*cp == ':' && !in_literal) {
			for (ph = ++cp, n = 0;
			     *cp && (isalnum(*cp) || *cp == '_')
			     && n < MAX_SQL_IDENTIFIER; cp++, n++);
			*cp = '\0';
			printf("Enter value for %s: ", ph);
			gets((char *) &bind_values[i][0]);

			/* Do the bind, using obndrv().
			   NOTE:  the bind variable address must be static.
			   This would not work if bind_values were an
			   auto on the do_binds stack. */
			if (obndrv
			    (cda, ph, -1, (ub1 *) & bind_values[i][0], -1,
			     VARCHAR2_TYPE, -1, (sb2 *) 0, (text *) 0, -1,
			     -1)) {
				oci_error(cda);
				return -1;
			}
			i++;
		}		/* end if (*cp == ...) */
	}			/* end for () */
	return i;
}



/*  Clean up and exit.  LDA and CDA are
    global. */
void do_exit(rv)
sword rv;
{
	if (oclose(&cda))
		fputs("Error closing cursor!\n", stdout);
	if (ologof(&lda))
		fputs("Error logging off!\n", stdout);
	exit(rv);
}


void oci_error(cda)
Cda_Def *cda;
{
	text msg[512];
	sword n;

	fputs("\n-- ORACLE ERROR --\n", stderr);
	n = oerhms(&lda, cda->rc, msg, (sword) sizeof(msg));
	fprintf(stderr, "%.*s", n, msg);
	fprintf(stderr, "Processing OCI function %s\n",
		oci_func_tab[cda->fc]);
	fprintf(stderr, "Do you want to continue? [yn]: ");
	fgets((char *) msg, (int) sizeof(msg), stdin);
	if (*msg != '\n' && *msg != 'y' && *msg != 'Y')
		do_exit(1);
	fputc('\n', stdout);
}


sword get_sql_statement()
{
	text cbuf[1024];
	text *cp;
	sword stmt_level;


	for (stmt_level = 1;;) {
		if (stmt_level == 1) {
			*sql_statement = '\0';
		} 
		if (gets((char *) cbuf) == NULL)
			return -1;
		if (*cbuf == '\0')
			continue;
		if (strncmp((char *) cbuf, "exit", 4) == 0)
			return -1;

		if (stmt_level > 1)
			strcat((char *) sql_statement, " ");
		strcat((char *) sql_statement, (char *) cbuf);

		cp = &sql_statement[strlen((char *) sql_statement) - 1];

		while (isspace(*cp))
			cp--;
		if (*cp == ';') {
			*cp = '\0';
			break;
		}
		stmt_level++;
	}
	return 0;
}


void print_header(ncols)
sword ncols;
{
	sword col, n, i;

	for (col = 0; col < ncols; col++) {
		for(i=0;desc[col].buf[i]!=' ';i++) 
			;
		desc[col].buf[i]='\0';
		printf("%s%c", desc[col].buf, col==ncols-1 ? '\n' : delim[0]);
	}
	
}


void print_rows(cda, ncols)
Cda_Def *cda;
sword ncols;
{
	sword col, n;

	for (;;) {
		/* Fetch a row.  Break on end of fetch,
		   disregard null fetch "error". */
		if (ofetch(cda)) {
			if (cda->rc == NO_DATA_FOUND)
				break;
			if (cda->rc != NULL_VALUE_RETURNED)
				oci_error(cda);
		}
		for (col = 0; col < ncols; col++) {
			/* Check col. return code for null.  If
			   null, print n spaces, else print value. */
			if (def[col].indp == -1)
				;
			else {
				switch (desc[col].dbtype) {
				case FLOAT_TYPE:
					printf("%*.*f", numwidth, 2,
					       def[col].flt_buf);
					break;
				case INT_TYPE:
					printf("%d",def[col].int_buf);
					break;
				case SQLT_LNG:
					def[col].buf[def[col].col_retlen]='\0';
					printf("%s", def[col].buf);
					if (def[col].indp == -2
					    || def[col].col_retlen <
					    def[col].indp)
						fetch_long(cda, col);
					break;
				default:
					printf("%s", def[col].buf);
					break;
				}
			}
			if (col != ncols-1)
				fputc(delim[0], stdout);
		}
		fputc(rs, stdout);
	}			/* end for (;;) */
}

void fetch_long(Cda_Def * cda, sword col)
{

	ub4 offset = def[col].col_retlen;
	/* we have already fetched col2_rlen characters so this is where the offset
	   starts  for the next fetch cycle */
	ub4 piece = 0;


	do {

		if (oflng(cda, col + 1, (ub1 *) def[col].buf,
		     (sb4) (sizeof(def[col].buf) - 1), SQLT_LNG, &piece, offset)) {
			oci_error(cda);
			return;
		}

		if (piece) {	/* did we get anything? */
			def[col].buf[piece] = '\0';
			printf("%s", def[col].buf);
		}

		offset += piece;	/* bump the offset and keep going */

	} while (piece == LONG_SIZE);

}
