/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>
#include <fatal.h>

void *
zalloc(ulong size, int n) {
	void *x;

	if ((x = mallocz(size, n)) == nil)
		fatal("mallocz");
	return x;
}
