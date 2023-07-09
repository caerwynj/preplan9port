#include <u.h>
#include <libc.h>
#include <libg.h>
#include <frame.h>

int main(int argc, char *argv[])
{
	Rectangle r;

	xtbinit(0, "win", &argc, argv, 0);
	r = inset(screen.r, 4);
	bitblt(&screen, r.min, &screen, r, 0);
	einit(Emouse|Ekeyboard);
	for (;;)
		sleep(10);
}

void
ereshaped(Rectangle r)
{
	r = inset(r, 4);
}

