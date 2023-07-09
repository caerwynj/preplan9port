/*
 * Sape Mullender, January 2000
 *
 * Copyright (c) 2000 Lucent Technologies Bell Labs Innovations
 * All rights reserved.
 */

typedef struct Lock Lock;

struct Lock {
	long l;
};

void lock(Lock *);
void unlock(Lock *);
