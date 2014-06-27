include ../common.mk


# sources, headers, objects
CUNIT_SOURCES = \
				ntru_cunit.c \
				ntru_file_cunit.c \
				ntru_poly_cunit.c \
				ntru_keypair_cunit.c \
				ntru_encrypt_cunit.c \
				ntru_decrypt_cunit.c

CUNIT_OBJS = $(patsubst %.c, %.o, $(CUNIT_SOURCES))

CUNIT_HEADERS = \
				ntru_cunit.h


# libs
LIBS += -L. -lcunit -llz4 -lgmp -lmpfr -lflint \
		$(shell $(PKG_CONFIG) --libs glib-2.0) -lm

# includes
INCS = -I. -I../include -I/usr/include/flint $(shell $(PKG_CONFIG) --cflags glib-2.0)

CFLAGS += -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED


%.o: %.c
	$(CC) -fPIC $(CFLAGS) $(CPPFLAGS) $(INCS) -c $*.c

check: ntru_cunit
	./ntru_cunit

test: ntru_cunit

libpqc.a:
	$(MAKE) -C ../src libpqc.a

ntru_cunit: $(CUNIT_OBJS) $(CUNIT_HEADERS) libpqc.a
	$(CC) $(CFLAGS) -o $@ $(CUNIT_OBJS) ../src/libpqc.a $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o ntru_cunit *.orig core test-file.out pub.key


.PHONY: check clean test
