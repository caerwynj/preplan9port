CFLAGS=-I../../include -DSOLARIS -D_POSIX_SOURCE
LFLAGS=-L../../lib -lregexp -lbio -l9
CC=cc
TARG=mk

OFILES=	arc.o\
	archive.o\
	bufblock.o\
	dir.o\
	env.o\
	file.o\
	graph.o\
	job.o\
	lex.o\
	main.o\
	match.o\
	mk.o\
	parse.o\
	Posix.o\
	recipe.o\
	rule.o\
	run.o\
	sh.o\
	shprint.o\
	symtab.o\
	var.o\
	varsub.o\
	word.o\

HFILES=	fns.h\
	../../include/ar.h\
	mk.h\

${TARG}: ${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@
