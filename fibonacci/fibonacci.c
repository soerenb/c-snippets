/*
 * fibonacci.c - Calculate fibonacci series
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
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <foo.h>

/**
 * fibonacci_it() - Iterative fibonacci series calculation
 * @fib		Array to store fibonacci series
 * @limit	Number of fibonacci numbers to calculate
 */
static void fibonacci_it(uintptr_t *fib, size_t limit)
{
	fib[0] = 0;
	fib[1] = 1;
	for (uintptr_t i = 2; i < limit; i++)
		fib[i] = fib[i - 1] + fib[i - 2];
}

/**
 * fibonacci_rec() - Recursive fibonacci series calculation
 * @fib_rec	Array to store fibonacci series
 * @limit	Number of fibonacci numbers to calculate
 */
static uintptr_t fibonacci_rec(uintptr_t *fib_rec, size_t limit)
{
	uintptr_t ret;

	if (limit == 0) {
		fib_rec[0] = 0;
		return 0;
	}
	if (limit == 1) {
		fib_rec[1] = 1;
		return 1;
	}

	ret = (fibonacci_rec(fib_rec, limit - 2) +
			fibonacci_rec(fib_rec, limit - 1));
	fib_rec[limit] = ret;
	return ret;
}

int main(int argc, char *argv[])
{
	char *buf;
	size_t limit = 48;
	uintptr_t *fib_it;
	uintptr_t *fib_rec;

	if (argc > 2) {
		printf("usage:\n\t%s [<limit>]\n", argv[0]);
		return -1;
	}

	if (argc == 2) {
		limit = strtol(argv[1], NULL, 0);
	} else {
		printf("Enter the number of elements to calculate (%zu): ",
				limit);
		buf = foo_gets();
		if ((buf != NULL) && strcmp(buf, "\n"))
			limit = strtol(buf, NULL, 0);
		free(buf);
	}
	if (limit < 2)
		limit = 2;

	fib_it = malloc(limit * sizeof(*fib_it));
	fib_rec = malloc(limit * sizeof(*fib_rec));

	/* Calculating */
	printf("Calculating the first %zu Fibonacci numbers\n", limit);
	fibonacci_it(fib_it, limit);
	fibonacci_rec(fib_rec, limit - 1);

	/* Printing series */
	printf("Iterative calculated:\n");
	printf("F[%u] = %" PRIuPTR "\n", 0, fib_it[0]);
	for (uintptr_t i = 1; i < limit; i++) {
		if (fib_it[i] < fib_it[i - 1]) {
			printf("Integer overflow detected at F[%" PRIuPTR "].\n",
			       i);
			break;
		}
		printf("F[%" PRIuPTR "] = %" PRIuPTR "\n", i, fib_it[i]);
	}
	printf("\n");
	printf("Recursive calculated:\n");
	printf("F[%u] = %" PRIuPTR "\n", 0, fib_rec[0]);
	for (uintptr_t i = 1; i < limit; i++) {
		if (fib_rec[i] < fib_rec[i - 1]) {
			printf("Integer overflow detected at F[%" PRIuPTR "].\n",
			       i);
			break;
		}
		printf("F[%" PRIuPTR "] = %" PRIuPTR "\n", i, fib_rec[i]);
	}

	free(fib_it);
	free(fib_rec);

	return 0;
}
