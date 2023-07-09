#include <u.h>
#include <libc.h>
#include <libg.h>

void
ereshaped(Rectangle r)
{
	Point p;

	screen.r = r;
	bitblt(&screen, screen.r.min, &screen, r, Zero);
	p.x = screen.r.min.x + Dx(screen.r)/2;
	p.y = screen.r.min.y + Dy(screen.r)/2;
	p = sub(p, divpt(strsize(font, "hello world"),  2));
	string(&screen, p, font, "hello world", S);
}

int main(int argc, char *argv[])
{
	Mouse m;

	xtbinit(0, 0, &argc, argv, 0);
	ereshaped(screen.r);
	einit(Emouse|Ekeyboard);
	for(;;){
		m = emouse();
		if(m.buttons & 1)
			break;
		if(m.buttons & 4){
			string(&screen, m.xy, font, "hello world", S);
			do; while(emouse().buttons & 4);
		}
	}
}


