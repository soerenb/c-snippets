CC  = gcc
CXX = g++
LD  = ${CC}

CFLAGS   = -Wall -O3 -march=native -I../libfoo/include
LDFLAGS  = ${CFLAGS} -L../libfoo/lib
LDLIBS = -lfoo

SOURCES = pal.c

include ../Makefile.include
