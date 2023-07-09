SHELL=/bin/sh
CC = lcc
LINK = $(CC)
INCLUDES= -I../../include  -I/usr/openwin/include -Ilibmsg
OPTS=-DSOLARIS -D_POSIX_SOURCE  -D_LIBXG_EXTENSION -DNDEBUG
CFLAGS = -g $(OPTS) $(INCLUDES)
LDFLAGS =-L../../lib -L/usr/openwin/lib
LIBS=-lframe -lXg -lstdio -l9 -lnsl -lsocket -lXt -lSM -lICE -lX11 -lsocket  -lnsl

TARGET=a.out
OBJECTS=\
	adjust.o\
	builtins.o\
	click.o\
	col.o\
	cursor.o\
	data.o\
	dir.o\
	env.o\
	event.o\
	exec.o\
	file.o\
	font.o\
	global.o\
	grow.o\
	include.o\
	keyboard.o\
	label.o\
	line.o\
	list.o\
	mouse.o\
	msg.o\
	path.o\
	place.o\
	point.o\
	regexp.o\
	sam.o\
	scroll.o\
	search.o\
	select.o\
	tag.o\
	tagmatch.o\
	text.o\
	text2.o\
	tile.o\
	undo.o\
	util.o\
	vgeom.o\
	view.o\
	vsearch.o\
	vshow.o\
	wily.o\
	win.o\

MYLIBS=libmsg/libmsg.a

all: $(TARGET)
$(TARGET): $(OBJECTS) $(MYLIBS)
	$(LINK) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(MYLIBS) $(LIBS)

$(OBJECTS): wily.h const.h proto.h 

builtins.o keyboard.o mouse.o tag.o text.o text2.o tile.o view.o: view.h
keyboard.o search.o tag.o text.o text2.o undo.o view.o line.o click.o : text.h
tile.o grow.o adjust.o win.o col.o list.o:	tile.h
point.o:	tile.h view.h
sam.o regexp.o: sam.h
file.o msg.o data.o label.o :	data.h
adjust.o builtins.o keyboard.o mouse.o point.o select.o tag.o text2.o tile.o vgeom.o view.o vsearch.o vshow.o : view.h

clean:
	rm -f *.o core
nuke: clean
	rm -f $(TARGET)
