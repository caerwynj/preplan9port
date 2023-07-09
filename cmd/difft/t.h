#define NUM_TYPES 5		/* maximum levels in the hierarchy */

enum {
	SCHEMA_VLEN = 256,
	VALUE_LEN = 255
};

extern char *diff_state[];

enum {
	DIFF = -1,
	NO_DIFF,
	REMOVED,
	EXISTS,
	NEW,
	NUM_DIFFS
};

typedef struct Schema Schema;
struct Schema {
	int type;
	char *subtype;
	char *value;
	int diff;
	Schema *list;
	Schema *next;
};

void freeall_schema(Schema * p);
Schema *delitem_schema(Schema * p, char *value, int type, char *subtype);

Schema *new_schema(int type, char *subtype, char *value, int diff);
Schema *lookup_schema(Schema * p, char *value, int type, char *subtype);
Schema *add_schema(Schema * p, Schema * newp);

Schema *next_schema(Schema * p);
Schema *list_schema(Schema * p);

Schema *read_schema(FILE * fp);
void write_schema(Schema * p, FILE * fp);

/* Sets the diff value of old schema to removed and new schema to NEW
 * it then overlays the new schema onto the old. Both schemas are modified
 * the overlaid old schema is returned.
 */
Schema *diff_schema(Schema * old, Schema * new);

void walk_schema(Schema * treep, void (*fn) (Schema *, void *), void *arg);

void print_schema(Schema * p);
int count_schema(Schema * p);
