#include <u.h>
#include <libc.h>
#include "threadimpl.h"

void
main () {
	int i;
	for (i = 0; i < 10; i++) {
		setthread((Thread*)i);
		print("%lux\n", getthread());
	}
}
