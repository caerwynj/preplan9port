#include <unistd.h>
#include <stdio.h>

void main(int argc, char argv[])
{
	int fd;

	if(fd=open("t2", 2)){
		int n=lseek(fd, 0, 2);
		fprintf(stderr, "end %d\n", n);
		
	}
}
