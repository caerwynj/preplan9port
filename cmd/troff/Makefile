CFLAGS=-DUNICODE -c
LFLAGS=
CC=cc
TARG=troff
OFILES=n1.o\
	n2.o\
	n3.o\
	n4.o\
	n5.o\
	t6.o\
	n6.o\
	n7.o\
	n8.o\
	n9.o\
	t10.o\
	n10.o\
	t11.o\
	ni.o\
	hytab.o\
	suftab.o\
	dwbinit.o\

HFILES=tdef.h\
	fns.h\
	ext.h\
	dwbinit.h\



TMACDIR='"lib/tmac/tmac."'
FONTDIR="lib/troff/font"
NTERMDIR='"lib/troff/term/tab."'
TEXHYPHENS='"/home/xcs0998/sys/lib/texmf/tex/generic/hyphen/hyphen.tex"'
DWBHOME='"/home/xcs0998/sys/"'
TDEVNAME='"utf"'
NDEVNAME='"utf"'

troff:	${OFILES}
	${CC} ${LFLAGS} ${OFILES} -o $@

ni.o:	ni.c ${HFILES}
	${CC} ${CFLAGS} -DTMACDIR'='${TMACDIR} ni.c

t10.o:	t10.c ${HFILES}
	${CC} ${CFLAGS} -DTDEVNAME'='${TDEVNAME} t10.c

n1.o:	n1.c ${HFILES}
	${CC} ${CFLAGS}  n1.c
#-DFONTDIR'='${FONTDIR} -DNTERMDIR'='${NTERMDIR} -DTEXHYPHENS'='${TEXHYPHENS} -DALTHYPHENS'='${TEXHYPHENS} -DDWBHOME'='${DWBHOME}
n10.o:	n10.c ${HFILES}
	${CC} ${CFLAGS} -DTDEVNAME'='${NDEVNAME} n10.c

n8.o:	n8.c ${HFILES}
	${CC} ${CFLAGS} -DTEXHYPHENS'='${TEXHYPHENS} n8.c

dwbinit.o:	dwbinit.c
	${CC} ${CFLAGS} -DDWBHOME'='${DWBHOME} dwbinit.c
