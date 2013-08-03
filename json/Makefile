CC  = gcc
LD  = ${CC}
JSONLINT = jsonlint

CFLAGS   = -Wall -O3 -march=native
LDFLAGS  = ${CFLAGS}
LDLIBS = -ljson-c

SOURCES = json.c

include ../Makefile.include

validate-json: input.json schema.json
	$(JSONLINT) input.json > /dev/null
	$(JSONLINT) schema.json > /dev/null
	$(JSONLINT) -V schema.json input.json > /dev/null
