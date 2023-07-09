/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */

/* csv.h: interface for csv library */

#ifndef CSV_H_
#define CSV_H_

extern char *csvgetline(FILE * f);	/* read next input line */
extern char *csvfield(int n);	/* return field n */
extern int csvnfield(void);	/* return number of fields */
int csvfmt(FILE * fp, char *fmt, ...);

#endif
