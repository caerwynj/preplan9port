OS=-D_POSIX_SOURCE -DSOLARIS
INCS=-I../../include
XLIBS=-L/usr/openwin/lib -lXt -lX11 -lsocket -lnsl

CFLAGS=$(OS) $(INCS) -D_LIBXG_EXTENSION

LIBS= ../../lib/libframe.a ../../lib/libXg.a ../../lib/lib9.a
CC=cc

OBJ=win.o

all:	win

win:	$(OBJ) $(LIBS)
	$(CC) -o win $(OBJ) $(LIBS) $(XLIBS)

clean:
	rm -f *.o core

nuke:	clean
	rm -f win


