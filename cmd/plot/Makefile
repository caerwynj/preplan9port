CFLAGS= -Wl-t -I/home/xcs0998/src/include -DSOLARIS -D_POSIX_SOURCE -D_LIBXG_EXTENSION
LFLAGS=-L/home/xcs0998/src/lib -lstdio -lbio -lXg -l9 -L/usr/openwin/lib -lXt -lX11 -lsocket -lnsl
CC=lcc
TARG=plot
OFILES=plot.o\

HFILES=plot.h\

LIB=libplot.a

$(TARG):	$(OFILES)
	$(CC) -o $(TARG) $(OFILES) $(LIB) $(LFLAGS)
