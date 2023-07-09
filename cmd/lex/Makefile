CFLAGS=-DSOLARIS -D_POSIX_SOURCE -I../../include
LFLAGS=-L../../lib -lbio -l9 -lm
CC=cc
TARG=lex
OFILES=lmain.o\
	y.tab.o\
	sub1.o\
	sub2.o\
	header.o

HFILES=ldefs.h

YFILES=parser.y

${TARG}:${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@
