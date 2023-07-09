CFLAGS=-DSOLARIS -D_POSIX_SOURCE -c -I ./ -I ../../include
LFLAGS=-L ../../lib -lbio -l9 -lm
CC=cc
TARG=sprog
OFILES=sprog.o\

HFILES =\
	code.h\

all:	lib sprog

${TARG}: ${OFILES}
	${CC} ${OFILES} ${LFLAGS}  -o $@

lib:	amspell brspell
	cp $? /home/xcs0998/lib

brspell: pcode  list british local stop
	./pcode list british local stop >brspell

amspell: pcode  list american local stop
	./pcode list american local stop >amspell

pcode:	pcode.o ${HFILES}
	${CC}  pcode.o ${LFLAGS} -o $@

