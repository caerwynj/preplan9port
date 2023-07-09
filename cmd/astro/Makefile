LFLAGS=-L../../lib -l9 -lbio -lm
CFLAGS=-DSOLARIS -D_POSIX_SOURCE -g -I../../include
CC=cc
OFILES=\
	moon.o\
	moont.o\
	satel.o\
	merct.o\
	search.o\
	dist.o\
	sunt.o\
	occ.o\
	main.o\
	sat.o\
	venus.o\
	pdate.o\
	init.o\
	star.o\
	merc.o\
	stars.o\
	comet.o\
	nutate.o\
	helio.o\
	mars.o\
	output.o\
	jup.o\
	geo.o\
	venust.o\
	cosadd.o\
	nutt.o\
	uran.o\
	nept.o\
	plut.o\
	sun.o\

HFILES=astro.h\

astro: ${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@

