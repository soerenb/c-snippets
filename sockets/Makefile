CC  = gcc
CXX = g++
LD  = ${CC}

CFLAGS   = -Wall -O3 -march=native
CXXFLAGS = ${CFLAGS}
LDFLAGS  = ${CFLAGS}
MKFLAGS = --no-print-directory
LDLIBS =

SOURCES = server.c

all:
	@make ${MKFLAGS} -f Makefile.server
	@make ${MKFLAGS} -f Makefile.client

clean:
	@make ${MKFLAGS} -f Makefile.server clean
	@make ${MKFLAGS} -f Makefile.client clean
	rm -rf	UDSDGCLNT UDSDGSRV
