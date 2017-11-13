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

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <foo.h>

static uintptr_t cb_shift(uintptr_t n)
{
	uintptr_t ret = 0;

	while (n) {
		ret += n & 1;
		n >>= 1;
	}

	return ret;
}

static uintptr_t cb_sub(uintptr_t n)
{
	uintptr_t ret = 0;

	while (n) {
		ret++;
		n &= (n - 1);
	}

	return ret;
}

static uintptr_t cb_gcc(uintptr_t n)
{
	return __builtin_popcount(n);
}

static uintptr_t cb_asm(uintptr_t n)
{
	uintptr_t ret;

	__asm__ volatile(
			"popcnt	%1, %0"		/* at&t dialect!! */
			: "=r"(ret)
			: "r"(n)
			: /* no clobber */
	);

	return ret;
}

static uintptr_t (* const cb_func[])(uintptr_t) = {
	cb_shift,
	cb_sub,
	cb_gcc,
	cb_asm,
};

int main(int argc, char *argv[])
{
	while (1) {
		char *input;
		uintptr_t num;
		uintptr_t result[ARRAY_SIZE(cb_func)];

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

		for (size_t i = 0; i < ARRAY_SIZE(cb_func); i++)
			result[i] = cb_func[i](num);

		for (size_t i = 1; i < ARRAY_SIZE(cb_func); i++) {
			if (result[i] != result[i - 1]) {
				printf("ERROR: results don't match\n");
				printf("  %#" PRIxPTR ": ", num);
				for (size_t j = 0; j < ARRAY_SIZE(cb_func); j++)
					printf("%" PRIuPTR "\t", result[j]);
				printf("\n");

				return -1;
			}
		}

		printf("  %#" PRIxPTR ": %" PRIuPTR "\n\n", num, result[0]);
	}
}
