#include <stdio.h>

int fac(long n);

int main(int argc, char *argv[])
{
	printf("%d\n", fac(atoi(argv[1])));
	return 0;
}

int fac(long n)
{
	return n > 1 ? fac(n-1) * n : 1;
}
