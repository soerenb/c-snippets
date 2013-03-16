CC  = gcc
CXX = g++
LD  = ${CC}

CFLAGS   = -Wall -O3 -march=native -I../libfoo/include -D_FORTIFY_SOURCE=2
CXXFLAGS = ${CFLAGS}
LDFLAGS  = ${CFLAGS} -L../libfoo/lib
LDLIBS = -lfoo

SOURCES = fork.c

include ../Makefile.include
