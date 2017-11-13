/*
 * atoi.c - Implementation of atoi
 * Copyright (C) 2015 - 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <foo.h>

static int my_atoi(const char *s)
{
	int ret = 0;
	bool neg = false;

	if (!s)
		return 0;

	/* skip initial white spaces */
	while (isspace(*s)) {
		s++;
	}

	if (*s == '-') {
		neg = true;
		s++;
	} else if (*s == '+') {
		s++;
	}

	while (*s >= '0' && *s <= '9') {
		ret = ret * 10 + *s++ - '0';
	}

	if (neg)
		ret *= -1;

	return ret;
}

static const char *test_pattern[] = {
	"0",
	"4",
	"-5",
	"---13",
	"+++1298",
	"      -123",
	"      +94",
	"			42",
	"\n-234",
	"0x20",
	"0000666",
	"-987 654",
	"123x456",
};

int main(int argc, char *argv[])
{
	size_t errors = 0;

	for (size_t i = 0; i < ARRAY_SIZE(test_pattern); i++) {
		const char *num_str = test_pattern[i];
		int ref = atoi(num_str);
		int num = my_atoi(num_str);

		if (num != ref) {
			errors++;
			fprintf(stderr, "ERROR: mismatch: in:%s, out:%d, ref:%d\n",
				num_str, num, ref);
		}
	}

	return errors;
}
