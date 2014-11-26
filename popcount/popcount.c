/*
 * popcount.c - Count set bits
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

static unsigned int cb_shift(unsigned int n)
{
	unsigned int ret = 0;

	while (n) {
		ret += n & 1;
		n >>= 1;
	}

	return ret;
}

static unsigned int cb_sub(unsigned int n)
{
	unsigned int ret = 0;

	while (n) {
		ret++;
		n &= (n - 1);
	}

	return ret;
}

static unsigned int cb_gcc(unsigned int n)
{
	return __builtin_popcount(n);
}

static unsigned int cb_asm(unsigned int n)
{
	unsigned int ret;

	__asm__ volatile (
			"popcnt	%0, %1"		/* intel dialect!! */
			: "=r"(ret)
			: "r"(n)
			: /* no clobber */
	);

	return ret;
}

static unsigned int (* const cb_func[])(unsigned int) = {
	cb_shift,
	cb_sub,
	cb_gcc,
	cb_asm,
};

int main(int argc, char *argv[])
{
	while (1) {
		int i;
		char *input;
		unsigned int num;
		unsigned int result[ARRAY_SIZE(cb_func)];

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

		for (i = 0; i < ARRAY_SIZE(cb_func); i++)
			result[i] = cb_func[i](num);

		for (i = 1; i < ARRAY_SIZE(cb_func); i++) {
			if (result[i] != result[i - 1]) {
				int j;
				printf("ERROR: results don't match\n");
				printf("  %#x: ", num);
				for (j = 0; j < ARRAY_SIZE(cb_func); j++)
					printf("%u\t", result[j]);
				printf("\n");

				return -1;
			}
		}

		printf("  %#x: %u\n\n", num, result[0]);
	}
}
