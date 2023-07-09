/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

#include <u.h>
#include <lib9.h>

void
main(int, char *[]) {

	rfork(RFREND);
	switch (rfork(RFPROC)) {
	case -1:
		print("Can's fork: %r\n");
		exits("fork");
	case 0:
		rendezvous((ulong)main, 1);
		print("rendezvous\n");
		exits(0);
	}
	rfork(RFREND);
	switch (rfork(RFPROC)) {
	case -1:
		print("Can's fork: %r\n");
		exits("fork");
	case 0:
		rendezvous((ulong)main, 1);
		print("rendezvous\n");
		exits(0);
	}
	print("done\n");
}
