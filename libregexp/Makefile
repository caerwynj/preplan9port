#	Copyright (c) 1998 Lucent Technologies - All rights reserved.
#
#	Prototype Solaris Makefile for libregexp
#	Courtesy of Ed Kubaitis
#
#	Define operating system type: -DSOLARIS
#	
#	Additionally, -D_POSIX_SOURCE (or its equivalent) may be specified
#	if your compiler supports posix-compatible compilation
OS=-D_POSIX_SOURCE -DSOLARIS

#	add -Iincludedir for any include directories that need to be searched
#	for posix header files (for UMIPS, add -I/usr/include/posix)
INCS=-I../include

#	add name of library orderer - use ":" if none exists
RANLIB=:

#	add name of librarian
AR=ar

CFLAGS=-c $(OS) $(INCS)

LIB=../lib/libregexp.a
CC=cc

OBJ=regaux.o regcomp.o regerror.o regexec.o regsub.o rregexec.o rregsub.o

all:	$(LIB)

$(LIB):	$(OBJ)
	$(AR) rv $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

clean:
	rm -f *.o a.out

nuke:	clean
	rm -f $(LIB)

install:	$(LIB)

test:	test.o $(LIB)
	$(CC) test.o $(LIB) ../libframe/libframe.a ../libXg/libXg.a

test2:	test2.o $(LIB)
	$(CC) test2.o $(LIB) ../libframe/libframe.a ../libXg/libXg.a

$(OBJ):	../include/u.h ../include/libc.h ../include/regexp.h regcomp.h
