#include "mplot.h"
void dpoint(double x, double y){
	bitblt(&screen, screen.r.min, &screen, Rect(SCX(x), SCY(y), SCX(x)+1, SCY(y)+1), F);
	move(x, y);
}
