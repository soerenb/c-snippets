CC=gcc
LD=${CC}
CFLAGS=-Wall -O3 -march=native -I../../libfoo/include
LDFLAGS=${CFLAGS} -L../../libfoo/lib
LDLIBS=-lfoo
SOURCES=perf_numbers.c

include ../../Makefile.include
