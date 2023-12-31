#pragma src "/sys/src/libmemlayer"
#pragma lib "libmemlayer.a"

typedef struct Memscreen Memscreen;
typedef void (*Refreshfn)(Memimage*, Rectangle, void*);

struct Memscreen
{
	Memimage	*frontmost;	/* frontmost layer on screen */
	Memimage	*rearmost;	/* rearmost layer on screen */
	Memimage	*image;		/* upon which all layers are drawn */
	Memimage	*fill;			/* if non-zero, picture to use when repainting */
};

struct Memlayer
{
	Rectangle		screenr;	/* true position of layer on screen */
	Point			delta;	/* add delta to go from image coords to screen */
	Memscreen	*screen;	/* screen this layer belongs to */
	Memimage	*front;	/* window in front of this one */
	Memimage	*rear;	/* window behind this one*/
	int		clear;	/* layer is fully visible */
	Memimage	*save;	/* save area for obscured parts */
	Refreshfn	refreshfn;		/* function to call to refresh obscured parts if save==nil */
	void		*refreshptr;	/* argument to refreshfn */
};

/*
 * This function accepts local coordinates
 */
int			memload(Memimage*, Rectangle, uchar*, int, int);

/*
 * All these functions accept screen coordinates, not local ones.
 */
void			_memlayerop(void (*fn)(Memimage*, Rectangle, Rectangle, void*, int), Memimage*, Rectangle, Rectangle, void*);
Memimage*	memlalloc(Memscreen*, Rectangle, Refreshfn, void*, ulong);
void			memldelete(Memimage*);
void			memlfree(Memimage*);
void			memltofront(Memimage*);
void			memltofrontn(Memimage**, int);
void			memltorear(Memimage*);
void			memltorearn(Memimage**, int);
int			memlsetrefresh(Memimage*, Refreshfn, void*);
void			memlhide(Memimage*, Rectangle);
void			memlexpose(Memimage*, Rectangle);
void			_memlsetclear(Memscreen*);
int			memlorigin(Memimage*, Point, Point);
void			memlnorefresh(Memimage*, Rectangle, void*);
