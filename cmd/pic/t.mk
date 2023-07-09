CFLAGS=-DUNICODE -c -I ./
LFLAGS=
CC=cc

TARG=pic
OFILES=picy.o\
	picl.o\
	main.o\
	print.o\
	misc.o\
	symtab.o\
	blockgen.o\
	boxgen.o\
	circgen.o\
	arcgen.o\
	linegen.o\
	movegen.o\
	textgen.o\
	input.o\
	for.o\
	pltroff.o\

HFILES=pic.h\
	y.tab.h\

YFILES=picy.y\


YFLAGS=-S -d

pic:  	${OFILES}
	${CC} ${LFLAGS} ${OFILES} -o $@

picy.c:	y.tab.c
	mv $prereq $target

picl.c:	picl.lx
	$LEX -t $prereq > $target

