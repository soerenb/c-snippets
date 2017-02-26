/*
 * strparse.c - String parsing
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
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	size_t token_no = 0;
	const char *delim = "-:";

	if (argc < 2) {
		printf("Usage: %s <string> [<delim>]\n", argv[0]);
		return 1;
	}

	if (argc >=3) {
		delim = argv[2];
	}

	printf("input string: %s, delimiter: %s\n", argv[1], delim);

	/* strtok */
	printf("--- strtok ---\n");
	char *s = strdupa(argv[1]);
	if (!s) {
		fprintf(stderr, "unable to strdup\n");
		return 2;
	}

	s = strtok(s, delim);
	if (!s) {
		return 0;
	}

	do {
		token_no++;
		printf("token#%zu: %s\n", token_no, s);
	} while ((s = strtok(NULL, delim)));

	/* strpbrk */
	printf("--- strpbrk ---\n");
	token_no = 0;
	char *end, *start = strdupa(argv[1]);
	if (!start) {
		fprintf(stderr, "unable to strdup\n");
		return 2;
	}

	do {
		char d;

		token_no++;
		end = strpbrk(start, delim);
		if (end) {
			d = *end;
			*end = '\0';
		} else {
			d = '\0';
		}
		printf("delim: %c, token#%zu: %s\n", d, token_no, start);
		start = end;
	} while (start++);

	return 0;
}
