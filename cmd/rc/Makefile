# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4a from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = .
prefix = /home/xcs0998
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/rc
pkglibdir = $(libdir)/rc
pkgincludedir = $(includedir)/rc

top_builddir = .

ACLOCAL = /home/xcs0998/src/rc/missing aclocal
AUTOCONF = /home/xcs0998/src/rc/missing autoconf
AUTOMAKE = /home/xcs0998/src/rc/missing automake
AUTOHEADER = /home/xcs0998/src/rc/missing autoheader

INSTALL = ./install-sh -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
CC = cc
CPP = cc -E
CPPFLAGS = 
LN = ln
MAKEINFO = /home/xcs0998/src/rc/missing makeinfo
PACKAGE = rc
VERSION = 1.6

#ADDON = 
#man_MANS = rc.1 history.1
man_MANS = rc.1
#HISTORY = history

#EXECVE = execve.o

#READLINE = readline.o
#SYSTEM = system-bsd.o
SYSTEM = system.o

bin_PROGRAMS = rc
noinst_PROGRAMS = mksignal tripping $(HISTORY)

rc_SOURCES = builtins.c except.c exec.c fn.c footobar.c getopt.c glob.c glom.c hash.c heredoc.c input.c lex.c list.c main.c match.c nalloc.c open.c parse.c print.c redir.c signal.c status.c tree.c utils.c var.c wait.c walk.c which.c

EXTRA_rc_SOURCES = addon.c execve.c readline.c system.c system-bsd.c

rc_DEPENDENCIES = sigmsgs.o $(ADDON) $(EXECVE) $(READLINE) $(SYSTEM)
rc_LDADD = sigmsgs.o $(ADDON) $(EXECVE) $(READLINE) $(SYSTEM)

noinst_HEADERS = getgroups.h jbwrap.h parse.h proto.h rc.h rlimit.h wait.h

BUILT_SOURCES = sigmsgs.c

EXTRA_DIST = EXAMPLES RELDATE addon.c addon.h history.1 parse.y rc.1 trip.rc

CONFIGURE_DEPENDENCIES = RELDATE

DISTCLEANFILES = sigmsgs.c sigmsgs.h
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS) $(noinst_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I.
LDFLAGS = 
LIBS = 
rc_OBJECTS =  builtins.o except.o exec.o fn.o footobar.o getopt.o glob.o \
glom.o hash.o heredoc.o input.o lex.o list.o main.o match.o nalloc.o \
open.o parse.o print.o redir.o signal.o status.o tree.o utils.o var.o \
wait.o walk.o which.o
rc_LDFLAGS = 
mksignal_SOURCES = mksignal.c
mksignal_OBJECTS =  mksignal.o
mksignal_LDADD = $(LDADD)
mksignal_DEPENDENCIES = 
mksignal_LDFLAGS = 
tripping_SOURCES = tripping.c
tripping_OBJECTS =  tripping.o
tripping_LDADD = $(LDADD)
tripping_DEPENDENCIES = 
tripping_LDFLAGS = 
history_SOURCES = history.c
history_OBJECTS =  history.o
history_LDADD = $(LDADD)
history_DEPENDENCIES = 
history_LDFLAGS = 
CFLAGS = -g
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
man1dir = $(mandir)/man1
MANS = $(man_MANS)

NROFF = nroff
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  README ./stamp-h.in AUTHORS COPYING ChangeLog INSTALL \
Makefile.am Makefile.in NEWS acconfig.h acinclude.m4 aclocal.m4 \
config.h.in configure configure.in install-sh missing mkinstalldirs


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
SOURCES = $(rc_SOURCES) $(EXTRA_rc_SOURCES) mksignal.c tripping.c history.c
OBJECTS = $(rc_OBJECTS) mksignal.o tripping.o history.o

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in  acinclude.m4
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/config.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) acconfig.h
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f config.h

maintainer-clean-hdr:

mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

rc: $(rc_OBJECTS) $(rc_DEPENDENCIES)
	@rm -f rc
	$(LINK) $(rc_LDFLAGS) $(rc_OBJECTS) $(rc_LDADD) $(LIBS)

mksignal: $(mksignal_OBJECTS) $(mksignal_DEPENDENCIES)
	@rm -f mksignal
	$(LINK) $(mksignal_LDFLAGS) $(mksignal_OBJECTS) $(mksignal_LDADD) $(LIBS)

tripping: $(tripping_OBJECTS) $(tripping_DEPENDENCIES)
	@rm -f tripping
	$(LINK) $(tripping_LDFLAGS) $(tripping_OBJECTS) $(tripping_LDADD) $(LIBS)

history: $(history_OBJECTS) $(history_DEPENDENCIES)
	@rm -f history
	$(LINK) $(history_LDFLAGS) $(history_OBJECTS) $(history_LDADD) $(LIBS)

install-man1:
	$(mkinstalldirs) $(DESTDIR)$(man1dir)
	@list='$(man1_MANS)'; \
	l2='$(man_MANS)'; for i in $$l2; do \
	  case "$$i" in \
	    *.1*) list="$$list $$i" ;; \
	  esac; \
	done; \
	for i in $$list; do \
	  if test -f $(srcdir)/$$i; then file=$(srcdir)/$$i; \
	  else file=$$i; fi; \
	  ext=`echo $$i | sed -e 's/^.*\\.//'`; \
	  inst=`echo $$i | sed -e 's/\\.[0-9a-z]*$$//'`; \
	  inst=`echo $$inst | sed '$(transform)'`.$$ext; \
	  echo " $(INSTALL_DATA) $$file $(DESTDIR)$(man1dir)/$$inst"; \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(man1dir)/$$inst; \
	done

uninstall-man1:
	@list='$(man1_MANS)'; \
	l2='$(man_MANS)'; for i in $$l2; do \
	  case "$$i" in \
	    *.1*) list="$$list $$i" ;; \
	  esac; \
	done; \
	for i in $$list; do \
	  ext=`echo $$i | sed -e 's/^.*\\.//'`; \
	  inst=`echo $$i | sed -e 's/\\.[0-9a-z]*$$//'`; \
	  inst=`echo $$inst | sed '$(transform)'`.$$ext; \
	  echo " rm -f $(DESTDIR)$(man1dir)/$$inst"; \
	  rm -f $(DESTDIR)$(man1dir)/$$inst; \
	done
install-man: $(MANS)
	@$(NORMAL_INSTALL)
	$(MAKE) $(AM_MAKEFLAGS) install-man1
uninstall-man:
	@$(NORMAL_UNINSTALL)
	$(MAKE) $(AM_MAKEFLAGS) uninstall-man1

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
builtins.o: builtins.c rc.h config.h proto.h addon.h jbwrap.h rlimit.h \
	sigmsgs.h
except.o: except.c rc.h config.h proto.h jbwrap.h
exec.o: exec.c rc.h config.h proto.h wait.h
fn.o: fn.c rc.h config.h proto.h sigmsgs.h
footobar.o: footobar.c rc.h config.h proto.h
getopt.o: getopt.c rc.h config.h proto.h
glob.o: glob.c rc.h config.h proto.h
glom.o: glom.c rc.h config.h proto.h
hash.o: hash.c rc.h config.h proto.h sigmsgs.h
heredoc.o: heredoc.c rc.h config.h proto.h
input.o: input.c rc.h config.h proto.h jbwrap.h
lex.o: lex.c rc.h config.h proto.h parse.h
list.o: list.c rc.h config.h proto.h
main.o: main.c rc.h config.h proto.h
match.o: match.c rc.h config.h proto.h
mksignal.o: mksignal.c
nalloc.o: nalloc.c rc.h config.h proto.h
open.o: open.c rc.h config.h proto.h
parse.o: parse.c rc.h config.h proto.h
print.o: print.c rc.h config.h proto.h
readline.o: readline.c rc.h config.h proto.h jbwrap.h
redir.o: redir.c rc.h config.h proto.h
sigmsgs.o: sigmsgs.c sigmsgs.h
signal.o: signal.c rc.h config.h proto.h sigmsgs.h jbwrap.h
status.o: status.c rc.h config.h proto.h sigmsgs.h wait.h
system.o: system.c rc.h config.h proto.h
tree.o: tree.c rc.h config.h proto.h
tripping.o: tripping.c
utils.o: utils.c rc.h config.h proto.h jbwrap.h
var.o: var.c rc.h config.h proto.h
wait.o: wait.c rc.h config.h proto.h wait.h
walk.o: walk.c rc.h config.h proto.h jbwrap.h
which.o: which.c rc.h config.h proto.h getgroups.h

info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
all-recursive-am: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am: install-binPROGRAMS
	@$(NORMAL_INSTALL)
	$(MAKE) $(AM_MAKEFLAGS) install-exec-hook
install-exec: install-exec-am

install-data-am: install-man
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS uninstall-man
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS) $(MANS) $(HEADERS) config.h
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir) $(DESTDIR)$(mandir)/man1


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*
	-test -z "$(DISTCLEANFILES)" || rm -f $(DISTCLEANFILES)

maintainer-clean-generic:
	-test -z "$(BUILT_SOURCES)" || rm -f $(BUILT_SOURCES)
mostlyclean-am:  mostlyclean-hdr mostlyclean-binPROGRAMS \
		mostlyclean-noinstPROGRAMS mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-hdr clean-binPROGRAMS clean-noinstPROGRAMS \
		clean-compile clean-tags clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-hdr distclean-binPROGRAMS \
		distclean-noinstPROGRAMS distclean-compile \
		distclean-tags distclean-generic clean-am

distclean: distclean-am
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-binPROGRAMS \
		maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-noinstPROGRAMS distclean-noinstPROGRAMS \
clean-noinstPROGRAMS maintainer-clean-noinstPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile install-man1 uninstall-man1 install-man \
uninstall-man tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck all-recursive-am install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


sigmsgs.c sigmsgs.h: mksignal
	./mksignal

# Of course, parse.c and parse.h depend on parse.y.  However, unless
# you're hacking on rc's grammar, it's not useful to have this
# dependency expressed, since the distributed parse.[ch] (generated with
# byacc, and lightly edited to remove a couple of gcc warnings) are
# portable (I hope).
#parse.c parse.h: $(srcdir)/parse.y
#	$(YACC) -d $(srcdir)/parse.y
#     mv y.tab.c parse.c
#     mv y.tab.h parse.h

trip: rc tripping
	./rc -p < $(srcdir)/trip.rc

install-exec-hook:
#	$(INSTALL_PROGRAM) history $(bindir)/- ;\
#	rm -f $(bindir)/--;  $(LN) $(bindir)/- $(bindir)/-- ;\
#	rm -f $(bindir)/-p;  $(LN) $(bindir)/- $(bindir)/-p ;\
#	rm -f $(bindir)/--p; $(LN) $(bindir)/- $(bindir)/--p

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
