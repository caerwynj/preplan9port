.SUFFIXES: .c .o .exe
CFLAGS= -Wl-t -I/home/xcs0998/src/include -DSOLARIS -D_POSIX_SOURCE -D_LIBXG_EXTENSION
LFLAGS=-L/home/xcs0998/Solaris/sparc/lib -lbio -l9
CC=lcc

.c.o:
	$(CC) $(CFLAGS) -c $*.c

.c.exe:
	$(CC) $(CFLAGS) -o $*.exe $(OBJECTS) $*.c $(LFLAGS)

.o.exe:
	$(CC) $(CFLAGS) -o $*.exe $(OBJECTS) $*.o $(LFLAGS)
