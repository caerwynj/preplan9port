#	Copyright (c) 1998 Lucent Technologies - All rights reserved.
#
#	Prototype Solaris Makefile for libXg
#	Courtesy of Ed Kubaitis
#
#	Define operating system type: -DSOLARIS
#	
#	Additionally, -D_POSIX_SOURCE (or its equivalent) may be specified
#	if your compiler supports posix-compatible compilation
OS=-D_POSIX_SOURCE -DSOLARIS

#	add -Iincludedir for any include directories that need to be searched
INCS=-I../include -I/usr/openwin/include 

#	set this if your X libraries are in different locations
#	or if you need extra libraries to load with X11 applications
XLIBS=/usr/openwin/lib/libXt.a /usr/openwin/lib/libX11.a

#	add name of library orderer - use ":" if none
RANLIB=:

#	add name of librarian
AR=ar

#	the name of the library
LIB=../lib/libXg.a

CFLAGS=$(OS) -D_LIBXG_EXTENSION $(INCS)
CC=cc

OBJS=	arc.o arith.o balloc.o bitblt.o bitbltclip.o border.o bscreenrect.o\
	circle.o clipline.o clipr.o copymasked.o cursorset.o cursorswitch.o\
	disc.o ellipse.o font.o gcs.o getrect.o gwin.o ldconvert.o latin1.o\
	mkfont.o menuhit.o point.o polysegment.o rdbitmap.o rdbitmapfile.o\
	rdfontfile.o rectclip.o segment.o string.o strwidth.o texture.o\
	wrbitmap.o wrbitmapfile.o xtbinit.o

all install:	$(LIB)
compile:	$(LIB)
test:	test.o
	$(CC) -o $@ $? $(LIB)  -L../lib -lstdio -l9 -lnsl -lsocket -lXt -lSM -lICE -lX11 -lsocket  -lnsl
	echo try running test
clean:
	rm -f *.o test

nuke:	clean
	rm -f $(LIB)

$(LIB):	$(OBJS)
	$(AR) rv $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

$(LIB)(%.o): %.o

$(OBJS):	../include/libg.h libgint.h ../include/libc.h
