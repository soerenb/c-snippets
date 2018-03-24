CC = gcc
LD = ${CC}

CFLAGS = -Wall -O3 -march=native -I../libfoo/include
CXXFLAGS = ${CFLAGS}
LDFLAGS = ${CFLAGS} -L../libfoo/lib
LDLIBS = -lfoo

SOURCES = fb.c

include ../Makefile.include
