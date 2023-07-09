CFLAGS= -I/home/xcs0998/src/include -DSOLARIS -D_POSIX_SOURCE 
LFLAGS=-L/home/xcs0998/src/lib -lstdio  -l9
CC=lcc
TARG=graph
OFILES=graph.o\
	subr.o\
	whoami.o\

$(TARG):	$(OFILES)
	$(CC) -o $(TARG) $(OFILES) $(LFLAGS)
