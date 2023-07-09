
LIB=../lib/libstdio.a
OFILES=\
	_IO_getc.o\
	_IO_putc.o\
	_dtoa.o\
	_fconv.o\
	clearerr.o\
	fclose.o\
	fdopen.o\
	feof.o\
	ferror.o\
	fflush.o\
	fgetc.o\
	fgetpos.o\
	fgets.o\
	fileno.o\
	fopen.o\
	fprintf.o\
	fputc.o\
	fputs.o\
	fread.o\
	freopen.o\
	fscanf.o\
	fseek.o\
	fsetpos.o\
	ftell.o\
	fwrite.o\
	getc.o\
	gets.o\
	printf.o\
	putc.o\
	puts.o\
	rewind.o\
	scanf.o\
	sclose.o\
	setbuf.o\
	setvbuf.o\
	sopenr.o\
	sopenw.o\
	sprintf.o\
	sscanf.o\
	strtod.o\
	tmpfile.o\
	tmpnam.o\
	ungetc.o\
	vfprintf.o\
	vfscanf.o\
	vprintf.o\
	vsprintf.o\

HFILES=

INCLUDES=-I../include

CFLAGS += $(INCLUDES) -DSOLARIS -D_POSIX_SOURCE -DIEEE_8087

CC=cc

$(LIB): $(OFILES)
	ar r $(LIB) $(OFILES)

clean:
	rm -rf $(TARG) $(OFILES)

world:;		make depend; make $(LIB)

depend::;	makedepend $(CPPFLAGS) $(INCLUDES) $(OFILES:.o=.c)

