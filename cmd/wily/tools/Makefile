include ../../../config.mk
CFLAGS= -Wl-t -I/home/xcs0998/src/include -DSOLARIS -D_POSIX_SOURCE -DHAVE_SYS_SELECT_H
LFLAGS=-L/home/xcs0998/src/lib -lstdio ../libmsg/libmsg.a  -lsocket -lnsl -l9

EXE=\
	Wexec.exe\
	Wgoto.exe\
	Wins.exe\
	wattach.exe\
	wcat.exe\
	win.exe\
	windows.exe\
	wread.exe\
	wreplace.exe\

BIN=/home/xcs0998/bin
include ../../../mkmany
