CFLAGS=-DSYSV -DLOG='"/tmp/u9fs.log"'

u9fs:	u9fs.o conv.o mfmt.o
	cc -o u9fs u9fs.o conv.o mfmt.o -lsocket -lnsl

%.o:	%.c  u.h libc.h 9p.h
	cc -c $(CFLAGS) $<

clean:
	rm -f *.o u9fs
