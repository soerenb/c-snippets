/*
 * bw.c - Bitwise tricks
 * Copyright (C) 2014  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <stdlib.h>
#include <errno.h>
#include <foo.h>

static unsigned int rm_rightmost(unsigned int n)
{
	return n & (n - 1);
}

static unsigned int ex_rightmost(unsigned int n)
{
	return n & (-n);
}

static unsigned int smear_left(unsigned int n)
{
	return n | (-n);
}

static unsigned int smear_right(unsigned int n)
{
	return n | (n - 1);
}

static unsigned int rm_smear_left(unsigned int n)
{
	return n ^ (-n);
}

static unsigned int ex_smear_right(unsigned int n)
{
	return n ^ (n - 1);
}

static unsigned int ex_rm_smear_right(unsigned int n)
{
	return ~n & (n - 1);
}

int main(int argc, char *argv[])
{
	while (1) {
		char *input;
		unsigned int num;

		printf("enter number (q to exit): ");
		input = foo_gets();
		if (!input) {
			printf("ERROR: Error while reading from stdin.\n");
			return -1;
		}
		if (input[0] == 'q') {
			free(input);
			return 0;
		}
		errno = 0;
		num = strtol(input, NULL, 0);
		if (errno == ERANGE)
			printf("WARNING: strtol conversion out of range.\n");
		free(input);

		printf("	     remove: %#x => %#x\n", num, rm_rightmost(num));
		printf("	    extract: %#x => %#x\n", num, ex_rightmost(num));
		printf("	 smear left: %#x => %#x\n", num, smear_left(num));
		printf("	smear right: %#x => %#x\n", num, smear_right(num));
		printf("remove & smear left: %#x => %#x\n", num, rm_smear_left(num));
		printf("extr. & smear right: %#x => %#x\n", num, ex_smear_right(num));
		printf("ex, rm, smear right: %#x => %#x\n", num, ex_rm_smear_right(num));
	}
}
