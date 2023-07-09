
COMMONDIR=../common
FONTDIR=/home/xcs0998/sys/lib/troff/font
POSTLIB=/home/xcs0998/sys/lib/postscript/prologues

TARG=tr2post
CFLAGS=-I../../../include -DSOLARIS -D_POSIX_SOURCE -c -D'PROGRAMVERSION="0.1"' -D'DOROUND=1' -I../common
LFLAGS=-L../../../lib -lregexp -l9 -lbio -lm
CC=cc

OFILES=tr2post.o\
	chartab.o\
	Bgetfield.o\
	conv.o\
	utils.o\
	devcntl.o\
	draw.o\
	readDESC.o\
	ps_include.o\
	pictures.o\
	../common/common.o\

HFILES=tr2post.h\
	ps_include.h\
	$COMMONDIR/common.h\
	$COMMONDIR/comments.h\
	$COMMONDIR/path.h\
	$COMMONDIR/ext.h\



${TARG}: ${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@

