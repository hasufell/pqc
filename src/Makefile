include ../common.mk


SOVER = 0
LIBVER = 0.0.1

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
LIBS += -L. -llz4 -lgmp -lmpfr -lflint $(shell $(PKG_CONFIG) --libs glib-2.0) -lm

# includes
INCS = -I. -I/usr/include/flint $(shell $(PKG_CONFIG) --cflags glib-2.0)

CFLAGS += -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED


%.o: %.c
	$(CC) -fPIC $(CFLAGS) $(CPPFLAGS) $(INCS) -c $*.c

all: libpqc.a libpqc.so.$(LIBVER)

libpqc.a: $(PQC_OBJS) $(PQC_HEADERS)
	$(AR) cru libpqc.a $(PQC_OBJS)

libpqc.so.$(LIBVER): libpqc.a $(PQC_HEADERS)
	$(CC) -shared $(CFLAGS) -Wl,-soname,libpqc$(SOVER) -o $@ $(LDFLAGS) \
		libpqc.a $(LIBS)

main: main.o libpqc.a
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) \
		main.o libpqc.a $(LIBS)

install:
	$(INSTALL_DIR) "$(DESTDIR)$(INSTALL_LIBDIR)"
	$(INSTALL_BIN) libpqc.so.$(LIBVER) "$(DESTDIR)$(INSTALL_LIBDIR)"
	$(LN_S) libpqc.so.$(LIBVER) "$(DESTDIR)$(INSTALL_LIBDIR)"/libpqc.so
	$(LN_S) libpqc.so.$(LIBVER) "$(DESTDIR)$(INSTALL_LIBDIR)"/libpqc.so.$(SOVER)

uninstall:
	$(RM) "$(DESTDIR)$(INSTALL_LIBDIR)/libpqc.so"
	$(RM) "$(DESTDIR)$(INSTALL_LIBDIR)/libpqc.so.$(SOVER)"
	$(RM) "$(DESTDIR)$(INSTALL_LIBDIR)/libpqc.so.$(LIBVER)"

doc:
	doxygen

doc-pdf: doc
	$(MAKE) -C latex pdf

clean:
	rm -rf html/ latex/ *.o test libpqc.a libpqc.so* main *.dec *.enc *.hex *.orig core


.PHONY: clean doc doc-pdf install uninstall
