
LIB=com.a
OFILES=bbox.o\
	glob.o\
	misc.o\
	request.o\
	rune.o\
	tempnam.o\
	getopt.o\

HFILES=comments.h\
	gen.h\
	ext.h\
	request.h\
	path.h\
	rune.h\

INCLUDES=-I../../../include

CFLAGS += $(INCLUDES) -DSOLARIS -D_POSIX_SOURCE

CC=cc

$(LIB): $(OFILES)
	ar r $(LIB) $(OFILES)

clean:
	rm -rf $(TARG) $(OFILES)

