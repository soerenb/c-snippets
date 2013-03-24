/*
 * ulb.c - Unset the lowest bit of a word
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <foo.h>

int main(int argc, char *argv[])
{
	uint32_t data;
	int mask;
	char *buf;

	while (1) {
		printf("Enter a number: ");
		buf = foo_gets();
		if (!buf) {
			printf("ERROR: Error reading from stdin.\n");
			continue;
		}
		if (!strcmp(buf, "exit\n") || !strcmp(buf, "quit\n")) {
			free(buf);
			return 0;
		}

		data = strtol(buf, NULL, 0);
		free(buf);

		printf("data = %#8.8x\n", data);
		if (data) {
			mask = 1;
			while (!(data & mask))
				mask <<= 1;
			data &= ~mask;
		}
		printf("data = %#8.8x\n", data);
	}

	return 0;
}
