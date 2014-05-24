# compiler, tools
CC = gcc
PKG_CONFIG ?= pkg-config

# flags
CFLAGS ?= -march=native -O2 -pipe
CFLAGS += -std=c99 -pedantic -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
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
LIBS += -L. -lgmp -lmpfr -lm

# objects
PQC_OBJS = poly.o mem.o encrypt.o decrypt.o keypair.o
PQC_HEADERS = err.h poly.h context.h encrypt.h decrypt.h keypair.h
# CUNIT_OBJS = cunit.o

# includes
INCS = -I.

ifndef UNBUNDLE
LIBFLINT = ../external/flint-2.4.3/libflint.a
INCS += -I../external/libtommath-0.42.0 \
		-I../external/libtompoly-0.04 \
		-I../external/flint-2.4.3
else
LIBFLINT = -lflint
endif


%.o: %.c
	$(CC) -fPIC $(CFLAGS) $(CPPFLAGS) $(INCS) -c $*.c

all: libpqc.a libpqc.so


# test: $(CUNIT_OBJS) $(PQC_LIBS)
	# $(CC) $(CFLAGS) -o $@ $(CUNIT_OBJS) $(LDFLAGS) $(CUNIT_LIBS) $(PQC_LIBS) $(LIBS)

ifndef UNBUNDLE
$(LIBFLINT):
	cd ../external/flint-2.4.3 && ./configure --prefix=/usr --with-gmp=/usr --with-mpfr=/usr --disable-shared
	$(MAKE) -C ../external/flint-2.4.3
endif

libpqc.a: $(PQC_OBJS) $(PQC_HEADERS)
	$(AR) cru libpqc.a $(PQC_OBJS)

libpqc.so: libpqc.a $(PQC_HEADERS) $(LIBFLINT)
	$(CC) -shared $(CFLAGS) -o $@ $(LDFLAGS) \
		libpqc.a $(LIBFLINT) $(LIBS)

main: main.o libpqc.a $(LIBFLINT)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) \
		main.o libpqc.a $(LIBFLINT) $(LIBS)

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
