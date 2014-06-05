include ../common.mk


# sources, headers, objects
PQC_SOURCES = \
			  ntru_ascii_poly.c \
			  ntru_decrypt.c \
			  ntru_encrypt.c \
			  ntru_file.c \
			  ntru_keypair.c \
			  ntru_mem.c \
			  ntru_poly.c \
			  ntru_poly_ascii.c \
			  ntru_rnd.c \
			  ntru_string.c

PQC_OBJS = $(patsubst %.c, %.o, $(PQC_SOURCES))

PQC_HEADERS = \
			  ntru_ascii_poly.h \
			  ntru_decrypt.h \
			  ntru_encrypt.h \
			  ntru_err.h \
			  ntru_file.h \
			  ntru_keypair.h \
			  ntru_poly.h \
			  ntru_params.h \
			  ntru_poly_ascii.h \
			  ntru_rnd.h \
			  ntru_string.h

# libs
LIBS += -L. -lgmp -lmpfr -lflint $(shell $(PKG_CONFIG) --libs glib-2.0) -lm

# includes
INCS = -I. -I/usr/include/flint $(shell $(PKG_CONFIG) --cflags glib-2.0)

CFLAGS += -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED


%.o: %.c
	$(CC) -fPIC $(CFLAGS) $(CPPFLAGS) $(INCS) -c $*.c

all: libpqc.a libpqc.so

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

clean:
	rm -f *.o test libpqc.a libpqc.so main *.dec *.enc *.hex *.orig core


.PHONY: clean install uninstall
