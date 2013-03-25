/*
 * unic.c - Playing with unicode/utf-8 chars
 * Copyright (C) 2013  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <stdio.h>
#include <wchar.h>
#include <stdint.h>

static const char foo[] = "foobar";
static const char jp[] = "ジおよびデバイ";
static const char palm[] = {0xf0, 0x9f, 0x8c, 0xb4, '\0'};
static const char smile[] = {0xf0, 0x9f, 0x98, 0x8e, '\0'};

int main(int argc, char *argv[])
{
	FILE *fout;
	int tmp;

	printf("%s\n", foo);
	printf("sizeof(foo): %lu\n", sizeof(foo));
	printf("%s\n", jp);
	printf("sizeof(japanese): %lu\n", sizeof(jp));
	printf("%s\n", palm);
	printf("sizeof(palm): %lu\n", sizeof(palm));
	printf("%s\n", smile);
	printf("sizeof(smile): %lu\n", sizeof(smile));

	fout = fopen("out.txt", "w");
	if (!fout) {
		printf("ERROR: Unable to open output file.\n");
		return -1;
	}

	tmp = fprintf(fout, "%s\n", jp);
	if (tmp != sizeof(jp))
		printf("WARNING: Unexpected number of character written.\n");

	if (fclose(fout))
		printf("WARNING: Error while closing output file.\n");
	
	return 0;
}
