
LIB=../lib/libplumb.a
OFILES=\
	mesg.o\

HFILES=../include/plumb.h

INCLUDES=-I../include

CFLAGS += $(INCLUDES) -DSOLARIS -D_POSIX_SOURCE

CC=lcc

$(LIB): $(OFILES)
	ar r $(LIB) $(OFILES)

clean:
	rm -rf $(TARG) $(OFILES)

