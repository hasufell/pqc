# compiler, tools
CC = $(shell type -P clang || echo gcc)
PKG_CONFIG ?= pkg-config

# flags
CFLAGS ?= -march=native -O2 -pipe
CFLAGS += -std=c99 -pedantic -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
ifeq ($(CC),gcc)
CFLAGS += -Wno-unused-but-set-variable
endif
LDFLAGS ?= -Wl,-O1 -Wl,--as-needed -Wl,--hash-style=gnu

# install variables
INSTALL = install
INSTALL_BIN = install -m755
INSTALL_DIR = install -d
PREFIX = /usr/local
LIBDIR = lib64
INSTALL_BINDIR = $(PREFIX)/bin
INSTALL_LIBDIR = $(PREFIX)/$(LIBDIR)
