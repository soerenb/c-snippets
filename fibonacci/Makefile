CC  = gcc
CXX = g++
LD  = ${CC}

CFLAGS = -Wall -O3 -march=native -I../libfoo/include
CXXFLAGS = ${CFLAGS}
LDFLAGS  = ${CFLAGS} -L../libfoo/lib
LDLIBS = -lfoo

SOURCES = fibonacci.c

include ../Makefile.include
