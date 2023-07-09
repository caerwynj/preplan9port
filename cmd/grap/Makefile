TARG=grap
OFILES=\
	grap.o\
	grapl.o\
	main.o\
	input.o\
	print.o\
	frame.o\
	for.o\
	coord.o\
	ticks.o\
	plot.o\
	label.o\
	misc.o\

HFILES=grap.h\
	y.tab.h\

YFILES=grap.y\

LFILES=grapl.lx\

CC=cc
LD=cc
LFLAGS= -lm
YFLAGS = -d -S
CFLAGS=-c

${TARG}: ${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@

grap.c:	grap.y
	yacc -d -S $?
	mv y.tab.c $@

grapl.c:	${LFILES}
	${LEX} -t $? > $@

