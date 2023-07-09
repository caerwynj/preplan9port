#include <u.h>
#include <libc.h>
#include <bio.h>

void main(void)
{
	int fd;
	Biobuf bin;

	if(fd=open("t2", 2)){
		vlong n;
		int x;
		Binit(&bin, fd, OREAD);
		x=Bseek(&bin, 0, 2);
		fprint(2, "end %d\n", x);
		x=lseek(fd, 0, 2);
		fprint(2, "end %d\n", x);
	}
}
