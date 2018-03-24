/*
 * fizzbuzz.c
 * Copyright (C) 2018  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <string.h>
#include <foo.h>

enum fb_result {
	FB_NONE,
	FB_FIZZ,
	FB_BUZZ,
	FB_FIZZBUZZ,
};

static int divides_by_n(size_t i, size_t n)
{
	if (!i) {
		return 0;
	}

	if (i % n) {
		return 0;
	}

	return 1;
}

static int divides_by_5(size_t i)
{
	return divides_by_n(i, 5);
}

static int divides_by_3(size_t i)
{
	return divides_by_n(i, 3);
}

static enum fb_result fizzbuzz(size_t i)
{
	if (!i) {
		return FB_NONE;
	}

	int fizz = divides_by_3(i);
	int buzz = divides_by_5(i);

	if (fizz && buzz) {
		return FB_FIZZBUZZ;
	}

	if (fizz) {
		return FB_FIZZ;
	}

	if (buzz) {
		return FB_BUZZ;
	}

	return FB_NONE;
}

int main(int argc, char *argv[])
{
	size_t lowerbound = 0;
	size_t upperbound = 100;

	printf("Enter lower bound for search (%zu): ", lowerbound);
	char *rline = foo_gets();
	if (rline == NULL) {
		printf("ERROR: Error reading from stdin");
	} else {
		if (strcmp(rline, "\n")) {
			lowerbound = strtol(rline, NULL, 0);
		}
		free(rline);
	}

	printf("Enter upper bound for search (%zu): ", upperbound);
	rline = foo_gets();
	if (!rline) {
		printf("ERROR: Error reading from stdin");
	} else {
		if (strcmp(rline, "\n")) {
			upperbound = strtol(rline, NULL, 0);
		}
		free(rline);
	}

	for (size_t i = lowerbound; i < upperbound; i++) {
		printf("%zu: ", i);
		switch (fizzbuzz(i)) {
		case FB_NONE:
			printf("%zu\n", i);
			break;
		case FB_FIZZ:
			printf("Fizz\n");
			break;
		case FB_BUZZ:
			printf("Buzz\n");
			break;
		case FB_FIZZBUZZ:
			printf("FizzBuzz\n");
			break;
		default:
			break;
		}
	}

	return 0;
}
