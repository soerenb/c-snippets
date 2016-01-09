CC=gcc
LD=${CC}
CFLAGS=-Wall -O3 -march=native -I../libfoo/include
LDFLAGS=${CFLAGS} -L../libfoo/lib
LDLIBS=-lfoo -lm
SOURCES=primes.c

include ../Makefile.include
