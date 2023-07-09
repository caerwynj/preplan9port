SHELL=/bin/sh
CC=lcc
CFLAGS=-DSOLARIS -D_POSIX_SOURCE -DHAVE_UNISTD_H -I../../../include  -I./ -I../
AR=ar
LIB=libmsg.a
OBJS=connect.o msg.o util.o rpc.o

all:	$(LIB)
clean:
	rm -f *.o
nuke:	clean
	rm -f $(LIB)
$(LIB):	$(OBJS)
	$(AR) rv $(LIB) $(OBJS)

