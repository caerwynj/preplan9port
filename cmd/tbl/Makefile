CFLAGS=-DSOLARIS -D_POSIX_SOURCE -c -I ./ -I../../include
LFLAGS=-L ../../lib -lbio -l9 -lm
CC=cc
TARG=tbl
OFILES=\
	t8.o\
	t4.o\
	t6.o\
	tu.o\
	t5.o\
	t7.o\
	tv.o\
	tg.o\
	t3.o\
	tb.o\
	tt.o\
	t9.o\
	t1.o\
	tf.o\
	tc.o\
	ti.o\
	tm.o\
	t0.o\
	tr.o\
	te.o\
	ts.o\
	t2.o\

HFILES=\
	t.h\

${TARG}: ${OFILES}
	${CC} ${OFILES} ${LFLAGS} -o $@
