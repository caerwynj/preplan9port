LIB=../lib/lib9.a

OFILES=\
	_assert.o\
	abs.o\
	argv0.o \
	asin.o\
	atan.o\
	atan2.o\
	atnotify.o\
	atof.o\
	atol.o\
	atoll.o\
	charstod.o\
	cleanname.o\
	conv.o\
	ctime.o\
	ctype.o\
	dirmodeconv.o\
	dirstat.o\
	doprint.o\
	execl.o\
	exp.o\
	fabs.o\
	fastrand.o\
	fatal.o\
	floor.o\
	fmod.o\
	getfields.o\
	getuser.o\
	hangup.o\
	hypot.o\
	lock.o\
	log.o\
	memccpy.o\
	memchr.o\
	memcmp.o\
	memmove.o\
	memset.o\
	misc.o\
	mktemp.o\
	nan.o\
	perror.o\
	pow.o\
	print.o\
	profile.o\
	qsort.o\
	rand.o\
	readn.o\
	rune.o\
	runetype.o\
	sin.o\
	sinh.o\
	sqrt.o\
	strcat.o\
	strchr.o\
	strcmp.o\
	strcpy.o\
	strcspn.o\
	strdup.o\
	strlen.o\
	strncat.o\
	strncmp.o\
	strncpy.o\
	strpbrk.o\
	strrchr.o\
	strspn.o\
	strstr.o\
	strtod.o\
	strtok.o\
	strtol.o\
	strtoul.o\
	tan.o\
	tanh.o\
	tokenize.o\
	toupper.o\
	utflen.o\
	utfnlen.o\
	utfrrune.o\
	utfrune.o\
	utfutf.o\

REJECT=	crypt.o\
	getcallerpc.o\
	malloc.o\
	netcrypt.o\
	netmkaddr.o\
	pool.o\
	pow10.o\
	wait.o\
	strtoull.o\
	strtoll.o\

HFILES=../include/libc.h
INCLUDES=-I../include
CFLAGS += $(INCLUDES) -DSOLARIS -D_POSIX_SOURCE -DLOCKS_AVAILABLE
CC=lcc

all: $(LIB)

$(LIB): $(OFILES)
	ar r $(LIB) $(OFILES)

clean:
	rm -rf $(OFILES)

