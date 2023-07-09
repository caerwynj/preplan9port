/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include "threadimpl.h"

void
incref(Ref *r)
{
	/* returns the old value; */
	_xinc(&r->ref);
}

long
decref(Ref *r)
{
	/* returns the new value; */
	return _xdec(&r->ref);
}
