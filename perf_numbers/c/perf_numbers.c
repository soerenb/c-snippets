/*
 * perf_numbers.c - Program finding perfect numbers
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
#include <time.h>
#include <string.h>
#include <foo.h>

int main(int argc, char *argv[])
{
	char *rline;
	clock_t start = 0;
	clock_t stop = 0;
	uintptr_t lowerbound = 1;
	uintptr_t upperbound = 1000;

	printf("Searching for perfect numbers:\n");

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

	start = clock();

	for (uintptr_t i = lowerbound; i <= upperbound; i++) {
		uintptr_t divcount = 0;
		uintptr_t summe = 0;
		uintptr_t  divs[100] = { 0 };

		for (intptr_t j = i >> 1; j > 0; j--) {
			if ((i % j) == 0) {
				summe += j;
				divs[divcount++] = j;
			}
			if (summe > i)
				break;
		}
		if (summe == i) {
			printf("%" PRIuPTR "\t\tDivs: ", summe);
			for (uintptr_t j = 0; j < divcount; j++)
				printf("%" PRIuPTR " ", divs[j]);
			printf("\n");
		}
	}

	stop = clock();

	printf("Computation time: %f\n",
	       (double)(stop - start) / (double)CLOCKS_PER_SEC);
	return 0;
}
