CC = gcc
LD = ${CC}
CFLAGS = -Wall -O3 -march=native -masm=intel -I../libfoo/include
LDFLAGS = ${CFLAGS} -L../libfoo/lib
LDLIBS = -lfoo

SOURCES = popcount.c

include ../Makefile.include
