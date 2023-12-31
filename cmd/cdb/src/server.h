#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED
#include "nub.h"

/* $Id: server.h,v 1.5 1998/10/09 00:07:47 drh Exp $ */

#define messagecodes \
	xx(NUB_CONTINUE) \
	xx(NUB_QUIT) \
	xx(NUB_STARTUP) \
	xx(NUB_BREAK) \
	xx(NUB_FAULT) \
	xx(NUB_FETCH) \
	xx(NUB_STORE) \
	xx(NUB_FRAME) \
	xx(NUB_SRC)

typedef enum {		/* messages */
#define xx(name) name,
	messagecodes
#undef xx
} Header_T;

struct nub_fetch {
	int space;
	const void *address;
	int nbytes;
};

struct nub_store {
	int space;
	void *address;
	int nbytes;
	char buf[1024];
};

struct nub_frame {
	int n;
	Nub_state_T state;
};
#endif
