#	Copyright (c) 1998 Lucent Technologies - All rights reserved.
#
#	Prototype Solaris Makefile for libframe
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

CFLAGS=-c $(OS) $(INCS) -D_LIBXG_EXTENSION

LIB=../lib/libframe.a
CC=cc

OBJ=frbox.o frdelete.o frdraw.o frinit.o frinsert.o frptofchar.o\
	frselect.o frstr.o frutil.o misc.o

all:	$(LIB)

$(LIB):	$(OBJ)
	$(AR) rv $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

clean:
	rm -f *.o

nuke:	clean
	rm -f $(LIB)

install:	$(LIB)

$(OBJ):	../include/u.h ../include/libc.h ../include/frame.h
