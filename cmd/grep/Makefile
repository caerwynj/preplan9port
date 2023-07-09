CFLAGS=-DSOLARIS -D_POSIX_SOURCE -c -I ./ -I ../../include
LFLAGS=-L ../../lib -lbio -l9 -lm
CC=cc
TARG=grep
HFILES=\
	grep.h\

OFILES=\
	y.tab.o\
	main.o\
	comp.o\
	sub.o\

YFILES=grep.y\

${TARG}: ${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@

