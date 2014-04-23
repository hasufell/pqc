# compiler, tools
CC = gcc
PKG_CONFIG ?= pkg-config

# flags
CFLAGS ?= -march=native -O2 -pipe
CFLAGS += -std=c99 -pedantic -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter
ifeq ($(shell $(CC) -v 2>&1 | grep 'gcc version' &>/dev/null && echo 1),1)
CFLAGS += -Wno-unused-but-set-variable
endif
LDFLAGS ?= -Wl,-O1 -Wl,--as-needed -Wl,--hash-style=gnu
# ifndef NOSTATIC
# LDFLAGS += -static
# endif

ifndef NODEBUG
CFLAGS += -O0 -g3
endif

# install variables
INSTALL = install
INSTALL_BIN = install -m755
INSTALL_DIR = install -d
PREFIX = /usr/local
LIBDIR = lib64
INSTALL_BINDIR = $(PREFIX)/bin
INSTALL_LIBDIR = $(PREFIX)/$(LIBDIR)


# libs
ifdef SHARED_LIB
PQC_LIBS = libpqc.so
else
PQC_LIBS = libpqc.a
endif
# CUNIT_LIBS = -lcunit
LIBS += -L.

# objects
PQC_OBJS = rand.o poly.o
PQC_HEADERS = err.h rand.h poly.h context.h
# CUNIT_OBJS = cunit.o

# includes
INCS = -I.

ifndef UNBUNDLE
LIBTOMMATH = ../external/libtommath-0.42.0/libtommath.a
LIBTOMPOLY = ../external/libtompoly-0.04/libtompoly.a
INCS += -I../external/libtommath-0.42.0 -I../external/libtompoly-0.04
else
LIBTOMMATH = -ltommath
LIBTOMPOLY = -ltompoly
endif


%.o: %.c
	$(CC) -fPIC $(CFLAGS) $(CPPFLAGS) $(INCS) -c $*.c

all: libpqc.a libpqc.so


# test: $(CUNIT_OBJS) $(PQC_LIBS)
	# $(CC) $(CFLAGS) -o $@ $(CUNIT_OBJS) $(LDFLAGS) $(CUNIT_LIBS) $(PQC_LIBS) $(LIBS)

ifndef UNBUNDLE
$(LIBTOMMATH):
	$(MAKE) -C ../external/libtommath-0.42.0

$(LIBTOMPOLY):
	$(MAKE) -C ../external/libtompoly-0.04
endif

libpqc.a: $(PQC_OBJS) $(PQC_HEADERS)
	$(AR) cru libpqc.a $(PQC_OBJS)

libpqc.so: libpqc.a $(PQC_HEADERS) $(LIBTOMMATH) $(LIBTOMPOLY)
	$(CC) -shared $(CFLAGS) -o $@ $(LDFLAGS) \
		libpqc.a $(LIBTOMMATH) $(LIBTOMPOLY) $(LIBS)

main: main.o libpqc.a $(LIBTOMMATH) $(LIBTOMPOLY)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) \
		main.o $(LIBTOMPOLY) libpqc.a $(LIBTOMPOLY) $(LIBTOMMATH) $(LIBS)

install:
	$(INSTALL_DIR) "$(DESTDIR)$(INSTALL_BINDIR)"
	[ -e libpqc.so ] && $(INSTALL_DIR) "$(DESTDIR)$(INSTALL_LIBDIR)"
	[ -e libpqc.so ] && $(INSTALL_BIN) libpqc.so "$(DESTDIR)$(INSTALL_LIBDIR)"

uninstall:
	[ -e "$(DESTDIR)$(INSTALL_LIBDIR)/libpqc.so" ] && rm "$(DESTDIR)$(INSTALL_LIBDIR)/libpqc.so"

# check:
	# ./test

clean:
	rm -f *.o test libpqc.a libpqc.so main


.PHONY: clean install
