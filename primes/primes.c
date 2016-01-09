/*
 * primes.c - Find prime numbers
 * Copyright (C) 2016  Sören Brinkmann <soeren.brinkmann@gmail.com>
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

#include <foo.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
	char *rline;
	clock_t start, stop;
	uintptr_t lowerbound = 2;
	uintptr_t upperbound = 100;

	printf("Searching for primes:\n");

	printf("Enter lower bound for search (%" PRIuPTR "): ", lowerbound);
	rline = foo_gets();
	if (rline == NULL) {
		printf("ERROR: Error reading from stdin");
	} else {
		if (strcmp(rline, "\n"))
			lowerbound = strtol(rline, NULL, 0);
		free(rline);
	}

	printf("Enter upper bound for search (%" PRIuPTR "): ", upperbound);
	rline = foo_gets();
	if (!rline) {
		printf("ERROR: Error reading from stdin");
	} else {
		if (strcmp(rline, "\n"))
			upperbound = strtol(rline, NULL, 0);
		free(rline);
	}

	printf("Search range: %" PRIuPTR "..%" PRIuPTR "\n",
	       lowerbound, upperbound);

	/* catch 2 as a prime */
	if (lowerbound <= 2) {
		printf("2\n");
	}

	/* only odd numbers can be prime (ignoring 2) */
	lowerbound |= 1;

	start = clock();

	for (uintptr_t i = lowerbound; i <= upperbound; i += 2) {
		bool is_prime = true;

		for (uintptr_t j = 2; j <= sqrt(i); j++) {
			if (i % j == 0) {
				is_prime = false;
				break;
			}
		}

		if (!is_prime)
			continue;

		printf("%" PRIdPTR "\n", i);
	}

	stop = clock();

	printf("Computation time: %f\n",
	       (double)(stop - start) / (double)CLOCKS_PER_SEC);

	return 0;
}
