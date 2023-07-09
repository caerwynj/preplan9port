LIB9=/home/xcs0998/contrib/russ
CFLAGS=-I./ -I${LIB9}/include
LFLAGS=-L${LIB9}/lib -lbio -lfmt -lutf
CC=cc

all: tdblook tdbjoin tdbadd tdbcoin 9ls

btree.o:	btree.h
plan9.o:	lib9.h

tdbadd: tdbadd.o btree.o plan9.o
	${CC} tdbadd.o btree.o plan9.o ${LFLAGS} -o $@

tdbjoin: tdbjoin.o btree.o plan9.o
	${CC} tdbjoin.o btree.o plan9.o ${LFLAGS} -o $@

tdblook: tdblook.o btree.o plan9.o
	${CC} tdblook.o btree.o plan9.o ${LFLAGS} -o $@

tdbcoin: tdbcoin.o btree.o plan9.o
	${CC} tdbcoin.o btree.o plan9.o ${LFLAGS} -o $@

9ls: 9ls.o plan9.o
	${CC} 9ls.o plan9.o ${LFLAGS} -o $@

clean:
	rm -f *.o

tar:
	tar -cvf utea.tar *.[ch] Makefile README man tea tock range
	gzip utea.tar
