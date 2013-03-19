CC  = gcc
CXX = g++
LD  = ${CC}
CFLAGS   = -Wall -O3 -march=native -D_FORTIFY_SOURCE=2 -I../libfoo/include

CXXFLAGS = ${CFLAGS}
LDFLAGS  = ${CFLAGS} -L../libfoo/lib
LDLIBS = -lfoo

SOURCES = ll.c

include ../Makefile.include
