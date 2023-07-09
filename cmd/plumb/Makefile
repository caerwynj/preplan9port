include ../../config.mk

EXE=\
	plumb.exe\

OBJECTS=\
	rules.o\
	match.o\

BIN=/home/xcs0998/bin
include ../../mkmany

plumb.exe: plumb.o
	$(CC) plumb.o -L../../lib -lplumb $(LFLAGS) -o $@
