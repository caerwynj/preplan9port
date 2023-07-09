#include <u.h>
#include <libc.h>

extern	int errno;
char	*strerror(int errnum);

int
errstr(char *s) {
	strcpy(s, strerror(errno));
	return 0;
}

void
exits(char *message)
{

	if (message == 0)
		exit(0);
	else
		exit(1);
}
