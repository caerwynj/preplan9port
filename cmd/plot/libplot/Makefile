LIB=../libplot.a
OFILES=box.o\
	cfill.o\
	circ.o\
	closepl.o\
	color.o\
	disk.o\
	doublebuffer.o\
	dpoint.o\
	erase.o\
	fill.o\
	frame.o\
	grade.o\
	line.o\
	machdep.o\
	move.o\
	openpl.o\
	parabola.o\
	pen.o\
	poly.o\
	ppause.o\
	pprompt.o\
	range.o\
	rarc.o\
	restore.o\
	rmove.o\
	rvec.o\
	save.o\
	sbox.o\
	spline.o\
	subr.o\
	text.o\
	vec.o\
	whoami.o\

HFILES=mplot.h\
INCLUDES=../../../include
CFLAGS += -I$(INCLUDES) -DSOLARIS -D_POSIX_SOURCE -D_LIBXG_EXTENSION
CC=lcc

$(LIB): $(OFILES)
	ar r $(LIB) $(OFILES)

clean:
	rm -rf $(TARG) $(OFILES)

