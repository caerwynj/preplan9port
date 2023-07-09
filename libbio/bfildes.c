#include	<u.h>
#include	<libc.h>
#include	<bio.h>

int
Bfildes(Biobuf *b)
{
	return b->_h.fid;
}
