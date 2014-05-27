include ../common.mk


# libs
ifdef SHARED_LIB
PQC_LIBS = libpqc.so
else
PQC_LIBS = libpqc.a
endif
# CUNIT_LIBS = -lcunit
LIBS += -L. -lgmp -lmpfr -lm $(shell $(PKG_CONFIG) --libs glib-2.0)


# sources, headers, objects
PQC_SOURCES = poly.c \
			  mem.c \
			  encrypt.c \
			  decrypt.c \
			  keypair.c \
			  ascii_poly.c \
			  file.c \
			  ntru_string.c

PQC_OBJS = $(patsubst %.c, %.o, $(PQC_SOURCES))

PQC_HEADERS = err.h \
			  poly.h \
			  context.h \
			  encrypt.h \
			  decrypt.h \
			  keypair.h \
			  ascii_poly.h \
			  common.h \
			  file.h \
			  ntru_string.h

# includes
INCS = -I. $(shell $(PKG_CONFIG) --cflags glib-2.0)

ifndef UNBUNDLE
LIBFLINT = ../external/flint-2.4.3/libflint.a
INCS += -I../external/flint-2.4.3
else
LIBFLINT = -lflint
INCS += -I/usr/include/flint
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
