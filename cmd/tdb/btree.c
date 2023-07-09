
enum {M = 100;};
typedef int Item;
typedef struct{
	ulong typ;
	ulong tm;
	ulong a;
	ulong r;
	ulong b;
} key;

typedef struct STnode* link;
typedef struct {
	Key key;
	union { 
		link next;
		Item item;
	}
	ref;
} entry;

struct STnode {
	entry b[M]; 
	int m;
};

static link head;
static int H, N;

link
NEW()
{
	link x = malloc(sizeof *x);
	x->m = 0;
	return x;
}

void
STinit(int maxN)
{
	head = NEW();
	H = 0;
	N = 0;
}

Item
searchR(link h, Key v, int H)
{
	int j;
	if(H == 0)
		for(j = 0; j < h->m; j++)
			if(eq(v, h->b[j].key))
				return h->b[j].ref.item;
	if(H != 0)
		for(j = 0; j < h->m; j++)
			if((j+1 == h->m) || less(v, h->b[j+1].key))
				return searchR(h->b[j].ref.next, v, H-1);
	return NULLitem;
}

Item
STsearch(Key v)
{
	return searchR(head, v, H);
}

link
insertR(link h, Item item, int H)
{
	int i, j;
	Key v = key(item);
	entry x;
	link u;

	x.key = v;
	x.ref.item = item;

	if(H == 0)
		for(j = 0; j < h->m; j++)
			if(less(v, h->b[j].key))
				break;
	if(H != 0)
		for(j = 0; j < h->m; j++)
			if((j+1 == h->m) || less(v, h->b[j+1].key)){
				u = insertR(h->b[j++].ref.next, v, H-1);
				if(u == NULL)
					return NULL;
				x.key = u->b[0].key;
				x.ref.next = u;
				break;
			}
	for(i = ++(h->m); (i > j) && (i != M); i--)
		h->b[i] = h->[i-1];
	h->b[j] = x;
	if(h->m < M)
		return NULL;
	else
		return split(h);
}

void
STinsert(Item item)
{
	link t, u = insertR(head, item, H);

	if(u == NULL)
		return;
	t = NEW();
	t->m = 2;
	t->b[0].key = head->b[0].key;
	t->b[0].ref.next = head;
	t->b[1].key = u->b[0].key;
	t->b[1].ref.next = u;
	head = t;
	H++;
}

link
split(link h)
{
	int j;
	link t = NEW();

	for(j = 0; j < M/2; j++)
		t->b[j] = h->b[M/2+j];
	h->m = M/2;
	t->m = M/2;
	return t;
}
