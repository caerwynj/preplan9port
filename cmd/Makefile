include ../config.mk

REJECT=\
	hget\
	html2ms\
	ms2html\
	sort\
	units\

EXE=\
	ascii.exe\
	basename.exe\
	btree.exe\
	calendar.exe\
	cat.exe\
	cleanname.exe\
	clock.exe\
	cmp.exe\
	comm.exe\
	date.exe\
	deroff.exe\
	du.exe\
	echo.exe\
	factor.exe\
	fmt.exe\
	fortune.exe\
	freq.exe\
	history.exe\
	join.exe\
	look.exe\
	ls.exe\
	mc.exe\
	p.exe\
	pr.exe\
	primes.exe\
	secs.exe\
	seq.exe\
	split.exe\
	strings.exe\
	sum.exe\
	tail.exe\
	tr.exe\
	unicode.exe\
	uniq.exe\
	wc.exe\
	xd.exe\
	yacc.exe\

OBJECTS=

BIN=/home/xcs0998/bin
include ../mkmany
sed.exe: sed.o
	$(CC) sed.o -L../lib -l9 -lbio -lregexp -lm -o $@

join.exe: join.o
	$(CC) join.o -L../lib -lstdio $(LFLAGS) -o $@

secs.exe: secs.o
	$(CC) secs.o -L../lib -lstdio $(LFLAGS) -o $@

units.exe: units.o
	yacc -o units.c units.y
	cc units.o $(LFLAGS) -o $@

clock.exe: clock.o
	$(CC) clock.o -L/home/xcs0998/Solaris/sparc/lib -lstdio -lbio -lXg -l9 -L/usr/openwin/lib -lXt -lX11 -lsocket -lnsl -o $@
