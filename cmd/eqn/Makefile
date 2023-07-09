CFLAGS=-DUNICODE -c -I ./
LFLAGS=
CC=cc
TARG=eqn
OFILES=main.o\
	tuning.o\
	diacrit.o\
	eqnbox.o\
	font.o\
	fromto.o\
	funny.o\
	glob.o\
	integral.o\
	input.o\
	lex.o\
	lookup.o\
	mark.o\
	matrix.o\
	move.o\
	over.o\
	paren.o\
	pile.o\
	shift.o\
	size.o\
	sqrt.o\
	text.o\
	eqn.o\

YFILES=eqn.y\

HFILES=e.h\
	y.tab.h\

YFLAGS=-d -S
eqn: 	${OFILES}
	${CC} ${LFLAGS} ${OFILES} -o $@

eqn.c:	y.tab.c prevy.tab.h
	mv y.tab.c $target

prevy.tab.h:	y.tab.h
	rc -c 'cmp -s y.tab.h prevy.tab.h || cp y.tab.h prevy.tab.h'
