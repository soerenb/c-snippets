CROSS_COMPILE=arm-unknown-linux-gnueabi-
CC  = ${CROSS_COMPILE}gcc
CXX = ${CROSS_COMPILE}g++
LD  = ${CC}

CFLAGS = -Wall -O2 -nostdlib
ASFLAGS = -nostdlib
CXXFLAGS = ${CFLAGS}

SOURCES = shellcode.S

include ../../Makefile.include
