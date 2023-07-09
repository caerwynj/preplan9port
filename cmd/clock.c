#include <u.h>
#include <libc.h>
#include <libg.h>

Point
circlept(Point c, int r, int degrees)
{
	double rad;
	rad = (double) degrees * PI/180.0;
	c.x += cos(rad)*r;
	c.y -= sin(rad)*r;
	return c;
}

void
redraw(Bitmap *screen)
{
	static int tm, ntm;
	static Rectangle r;
	static Point c;
	static int rad;
	static Bitmap *im;
	int i;
	int anghr, angmin;
	static Tm tms;
	static Tm ntms;

	ntm = time(0);
	if(ntm == tm && eqrect(screen->r, r))
		return;

	ntms = *localtime(ntm);
	anghr = 90-(ntms.hour*5 + ntms.min/10)*6;
	angmin = 90-ntms.min*6;
	tm = ntm;
	tms = ntms;
	r = screen->r;
	c = divpt(add(r.min, r.max), 2);
	rad = Dx(r) < Dy(r) ? Dx(r) : Dy(r);
	rad /= 2;
	rad -= 8;

	bitblt(screen, screen->r.min, screen, r, Zero);
	for(i=0; i<12; i++)
		ellipse(screen, circlept(c, rad, i*(360/12)), 2, 2, ~0, S);

	segment(screen, c, circlept(c, (rad*3)/4, angmin), ~0, S);
	segment(screen, c, circlept(c, rad/2, anghr), ~0, S);

	bflush();
}

void
ereshaped(Rectangle r)
{
	redraw(&screen);
}

void
main(int argc, char**argv)
{
	Event e;
	Mouse m;
	Menu menu;
	char *mstr[] = {"exit", 0};
	int key, timer;
	int t;

	xtbinit(0,"clock", &argc, argv, 0);
	redraw(&screen);

	einit(Emouse);
	t = (30*1000);
	timer = etimer(0, t);

	menu.item = mstr;
	menu.lasthit = 0;
	for(;;) {
		key = event(&e);
		if(key == Emouse) {
			m = e.mouse;
			if(m.buttons & 4) {
				if(menuhit(3, &m, &menu) == 0)
					exits(0);
			}
		} else if(key == timer) {
			redraw(&screen);
		}
	}	
}
