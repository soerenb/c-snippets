CC  = gcc
CXX = g++
LD  = ${CC}

CFLAGS   = -Wall -O3 -march=native -I../../libfoo/include
LDFLAGS  = ${CFLAGS} -L../../libfoo/lib
LDLIBS = -lncursesw -lfoo

SOURCES = gol.c

include ../../Makefile.include
