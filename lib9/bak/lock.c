#include "u.h"
#include "libc.h"

int canlock(Lock *l){
	int r;

	r=mutex_trylock(l);
	return r?0:r;
}
