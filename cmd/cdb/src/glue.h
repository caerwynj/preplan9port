#ifndef GLUE_INCLUDED
#define GLUE_INCLUDED

/* $Id: glue.h,v 1.14 1999/01/12 22:42:52 drh Exp $ */

struct module {
	unsigned int uname;
	void **addresses;
};

struct sframe {
	struct sframe *up, *down;
	int func;
	int module;
	int ip;
};

#endif
