CC  = gcc
CXX = g++
LD  = ${CC}

CFLAGS   = -Wall -O3 -march=native
CXXFLAGS = ${CFLAGS}
LDFLAGS  = ${CFLAGS}
LDLIBS =

SOURCES = server.c

all:
	make -f Makefile.server
	make -f Makefile.client

clean:
	make -f Makefile.server clean
	make -f Makefile.client clean
	rm -rf	UDSDGCLNT UDSDGSRV
