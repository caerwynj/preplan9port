#include "mplot.h"
Bitmap *offscreen;
/*
 * Clear the window from x0, y0 to x1, y1 (inclusive) to color c
 */
void m_clrwin(int x0, int y0, int x1, int y1, int c){
	bitblt(offscreen, offscreen->r.min, offscreen, Rect(x0, y0, x1+1, y1+1) , Zero);
}
/*
 * Draw text between pointers p and q with first character centered at x, y.
 * Use color c.  Centered if cen is non-zero, right-justified if right is non-zero.
 * Returns the y coordinate for any following line of text.
 */
int m_text(int x, int y, char *p, char *q, int c, int cen, int right){
	Point tsize;
	USED(c);
	tsize=strsize(font, p);
	if(cen) x -= tsize.x/2;
	else if(right) x -= tsize.x;
	string(offscreen, Pt(x, y-tsize.y/2), font, p, S);
	return y+tsize.y;
}
/*
 * Draw the vector from x0, y0 to x1, y1 in color c.
 * Clipped by caller
 */
void m_vector(int x0, int y0, int x1, int y1, int c){
	segment(offscreen, Pt(x0, y0), Pt(x1, y1), c, F);
}
char *scanint(char *s, int *n){
	while(*s<'0' || '9'<*s){
		if(*s=='\0'){
			fprint(2, "plot: bad -Wxmin,ymin,xmax,ymax\n");
			exits("bad arg");
		}
		s++;
	}
	*n=0;
	while('0'<=*s && *s<='9'){
		*n=*n*10+*s-'0';
		s++;
	}
	return s;
}
char *rdenv(char *name){
	return getenv(name);
}
/*
 * Startup initialization
 */
void m_initialize(char *s){
	static int first=1;
	int dx, dy;
	USED(s);
	if(first){
		clipminx=mapminx=screen.r.min.x+4;
		clipminy=mapminy=screen.r.min.y+4;
		clipmaxx=mapmaxx=screen.r.max.x-5;
		clipmaxy=mapmaxy=screen.r.max.y-5;
		dx=clipmaxx-clipminx;
		dy=clipmaxy-clipminy;
		if(dx>dy){
			mapminx+=(dx-dy)/2;
			mapmaxx=mapminx+dy;
		}
		else{
			mapminy+=(dy-dx)/2;
			mapmaxy=mapminy+dx;
		}
		first=0;
		offscreen = &screen;
	}
}
/*
 * Clean up when finished
 */
void m_finish(void){
	m_swapbuf();
}
void m_swapbuf(void){
	if(offscreen != &screen)
		bitblt(&screen, screen.r.min, offscreen, offscreen->r, S);
}
void m_dblbuf(void){
	if(offscreen==&screen){
		offscreen=balloc(screen.r, screen.ldepth);
		if(offscreen==0){
			fprintf(stderr, "Can't double buffer\n");
			*offscreen=screen;
		}
	}
}
/* Assume colormap entry because
 * Use cache to avoid repeated allocation.
 */
 /*
struct{
	int		v;
	Image	*i;
}icache[32];
*/
Bitmap *
getcolor(int v)
{
/*
	Image *i;
	int j;

	for(j=0; j<nelem(icache); j++)
		if(icache[j].v==v && icache[j].i!=nil)
			return icache[j].i;

	i = allocimage(display, Rect(0, 0, 1, 1), RGB24, 1, v);
	if(i == nil){
		fprint(2, "plot: can't allocate image for color: %r\n");
		exits("allocimage");
	}
	for(j=0; j<nelem(icache); j++)
		if(icache[j].i == nil){
			icache[j].v = v;
			icache[j].i = i;
			break;
		}

	return i;
	*/
	return 0;
}
