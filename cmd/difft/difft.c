#include <stdio.h>
#include "t.h"
#include "csv.h"
#include "eprintf.h"
#include "assert.h"
#include "ctype.h"

void diffschema(FILE * ofp, FILE * nfp)
{
	Schema *oldp, *newp;

	oldp = read_schema(ofp);
	if (!oldp) {
		weprintf("read_schema returned NULL");
	}
	newp = read_schema(nfp);
	if (!newp) {
		weprintf("read_schema returned NULL");
	}

	oldp = diff_schema(oldp, newp);
	if (!oldp) {
		weprintf("diff_schema returned NULL");
	}
	write_schema(oldp, stdout);
}

int main(int argc, char *argv[])
{
	int i;
	char *line;
	FILE *ofp, *nfp;

	if (argc != 3) {
		fprintf(stderr, "usage: %s old_schema new_schema\n",
			argv[0]);
		exit(1);
	}
	setprogname("diffschema");
	ofp = fopen(argv[1], "r");
	nfp = fopen(argv[2], "r");
	if (ofp && nfp) {
		diffschema(ofp, nfp);
	} else {
		fprintf(stderr, "couldn't open file\n");
	}

	return 0;
}

char *diff_state[] = { "NO DIFF", "REMOVED", "EXISTS", "NEW" };

char *attr_types[] = { "NO TYPE", "TABLE", "COLUMN", "NULLABLE", "DEFAULT",
	"TYPE", "LENGTH", "PREC", "SCALE", "POSITION",
	"TRIG", "PROC", "VIEW", "USERT", "SYST", "PARENT",
	"COMMENT"
};

void walk_schema(Schema * treep, void (*fn) (Schema *, void *), void *arg)
{
	if (treep == NULL)
		return;
	walk_schema(treep->next, fn, arg);
	(*fn) (treep, arg);
	walk_schema(treep->list, fn, arg);
}

static void _print_schema(Schema * p, void *arg)
{
	char *fmt;

	fmt = (char *) arg;
	printf(fmt, p->subtype, p->value, diff_state[p->diff]);
}

static void _inccounter_schema(Schema * p, void *arg)
{
	int *ip;

	ip = (int *) arg;
	(*ip)++;
}

void _setall_diff(Schema * p, void *arg)
{
	int *i;
	i = (int *) arg;
	p->diff = *i;
}

void _write_d(Schema * p, void *arg)
{
	FILE *fp;

	fp = (FILE *) arg;
	/* if the value contains a double-quote or comma we need to 
	 * quote it before outputing to csv format */
	if (strchr(p->value, '"') || strchr(p->value, ',')
	    || strchr(p->value, ' '))
		csvfmt(fp, "dsqd", p->type, p->subtype, p->value, p->diff);
	else
		csvfmt(fp, "dssd", p->type, p->subtype, p->value, p->diff);
}

/* search the old list for the new item. If it's in the list and
 * the diff is REMOVED set to exists. If it's not int he list
 * add the item to the end of the list with diff set to NEW.
 * The diff flag says whether we do the diff compare or not. If
 * the flag == 0 then we do a diff compare else we set the diff
 * type to the value of diff.
 */
Schema *search_insert_schema(Schema ** oldp, Schema * newp, int diff)
{
	Schema *p;

	p = lookup_schema(*oldp, newp->value, newp->type, newp->subtype);
	if (p) {
		if ((diff == DIFF) && p->diff == REMOVED)
			p->diff = EXISTS;
	} else {
		p = new_schema(newp->type, newp->subtype, newp->value,
			       (diff == DIFF) ? NEW : diff);
		*oldp = add_schema(*oldp, p);
	}
	return p;
}

/* st_add_schema (state add schema). This funtion keeps track
 * of the last insert you did. So if you inserted a table, a pointer
 * to that table is stored. If you then inserted a column. The
 * column is inserted in the sublist of the previously inserted
 * table. The diff flag states whether or not to do a diff
 * comparison on insert.
 */
Schema *st_add_schema(Schema * newp, Schema * oldp, int diff)
{
	static Schema **keys[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
	Schema *p;

	assert(newp);
	assert(newp->type < NUM_TYPES && newp->type >= 0);
	assert(diff < NUM_DIFFS && diff >= -1);
	keys[0] = &oldp;
	assert(keys[newp->type]);
	p = search_insert_schema(keys[newp->type], newp, diff);
	keys[newp->type + 1] = &(p->list);
	return *keys[0];
}

Schema *st_new_schema(Schema * p, char *name, int type,
		      char *subtype, int diff)
{
	Schema new;

	new.value = name;
	new.type = type;
	new.subtype = subtype;
	return st_add_schema(&new, p, diff);
}

Schema *read_schema(FILE * fp)
{
	Schema *p = NULL;
	char *line, *endp, *str;
	int type = -1, diff = -1;
	char *subtype;
	static int last = -1;

	while ((line = csvgetline(fp)) != NULL) {
		if (csvnfield() == 4) {
			/* check the ranges of the input values */
			str = csvfield(0);
			type = strtol(str, &endp, 10);
			if (*endp != '\0' || str[0] == '\0'
			    || type >= NUM_TYPES || type < 0)
				eprintf("bad type field %s", str);

			if (type > (last + 1))
				eprintf
				    ("bad type field %s, hierarchy out of sequence",
				     str);
			else
				last = type;

			subtype = csvfield(1);

			str = csvfield(3);
			diff = strtol(str, &endp, 10);
			if (*endp != '\0' || str[0] == '\0'
			    || diff >= NUM_DIFFS || diff < 0)
				eprintf("bad diff field %s", str);

			p =
			    st_new_schema(p, csvfield(2), type, subtype,
					  diff);
		} else
			eprintf
			    ("read schema: wrong number of fields %d; expected %d: %s",
			     csvnfield(), 4, line);
	}
	return p;
}

void write_schema(Schema * p, FILE * fp)
{
	walk_schema(p, _write_d, fp);
}

int count_schema(Schema * p)
{
	int i = 0;

	walk_schema(p, _inccounter_schema, &i);
	return i;
}

void print_schema(Schema * p)
{
	walk_schema(p, _print_schema, "%s\t%s\t%s\n");
}

Schema *walk_diff(Schema * oldp, Schema * newp)
{
	if (newp == NULL)
		return oldp;
	oldp = walk_diff(oldp, newp->next);
	oldp = st_add_schema(newp, oldp, DIFF);
	oldp = walk_diff(oldp, newp->list);
	return oldp;
}

/* Sets the diff value of old schema to removed and new schema to NEW
 * it then overlays the new schema onto the old. Both schemas are modified
 * the overlaid old schema is returned.
 */
Schema *diff_schema(Schema * oldp, Schema * newp)
{
	int diff;
	Schema *p, *q, *r;

	diff = REMOVED;
	walk_schema(oldp, _setall_diff, &diff);
	diff = NEW;
	walk_schema(newp, _setall_diff, &diff);
	oldp = walk_diff(oldp, newp);
	return oldp;
}

Schema *new_schema(int type, char *subtype, char *value, int diff)
{
	Schema *newp;

	if ((newp = (Schema *) malloc(sizeof(Schema))) == NULL) {
#ifdef DEBUG
		fprintf(stderr, "malloc Schema failed\n");
		exit(1);
#endif
		return NULL;
	}
	newp->value = estrdup(value);
	newp->type = type;
	newp->subtype = estrdup(subtype);
	newp->diff = diff;
	newp->list = NULL;
	newp->next = NULL;
	return newp;
}

void freeall_schema(Schema * p)
{
	if (p == NULL)
		return;

	freeall_schema(p->list);
	freeall_schema(p->next);
	free(p->value);
	free(p->subtype);
	p->value = NULL;
	p->subtype = NULL;
	free(p);
	p = NULL;
}


/* delitem will also delete all the sublists of that item. */
Schema *delitem_schema(Schema * listp, char *value, int type,
		       char *subtype)
{
	Schema *p, *prev;
	prev = NULL;
	for (p = listp; p != NULL; p = p->next) {
		if ((p->type == type) && (strcmp(subtype, p->subtype) == 0)
		    && (strcmp(value, p->value) == 0)) {
			if (prev == NULL) {
				listp = p->next;
			} else {
				prev->next = p->next;
			}
			freeall_schema(p->list);
			free(p->value);
			free(p->subtype);
			free(p);
			return listp;
		}
		prev = p;
	}
	return NULL;
}


/* Searches the current schema list for the given value. */
Schema *lookup_schema(Schema * p, char *value, int type, char *subtype)
{
	if (p) {
		for (; p != NULL; p = p->next)
			if ((p->type == type)
			    && (strcmp(subtype, p->subtype) == 0)
			    && (strcmp(value, p->value) == 0))
				return p;
	}
	return NULL;
}

/* Inserts the value into the current schema list. It doesn't check for
 * duplicates. If the subtype is not within the subtype range for this
 * schema list eg.  a column inserted onto a table list, the
 * schema_error_no is set.  The input list is returned.
 */
Schema *add_schema(Schema * listp, Schema * newp)
{
	if (newp) {
		newp->next = listp;
		return newp;
	} else
		return listp;
}
