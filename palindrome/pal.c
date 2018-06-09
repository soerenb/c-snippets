/*
 * pal.c - Palindrome test
 * Copyright (C) 2015-2018  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <stdio.h>
#include <string.h>

/**
 * revstr_inplace - Reverse string
 * @s:	String to reverse
 *
 * The string @s is reversed in place.
 */
static int is_palindrome(const char *s)
{
	if (!s) {
		return 0;
	}

	size_t l = strlen(s);

	if (l < 2) {
		return 1;
	}

	for (size_t i = 0; i < l / 2; i++) {
		if (s[i] != s[l - i - 1]) {
			return 0;
		}
	}

	return 1;
}

int main(int argc, char *argv[])
{
	char *string;

	if (argc > 1) {
		string = argv[1];
	} else {
		printf("Enter string: ");
		string = foo_gets();
		if (string[strlen(string) - 1] == '\n') {
			string[strlen(string) - 1] = '\0';
		}
	}

	printf("%s: %s\n", string, is_palindrome(string) ? "yes" : "no");

	return 0;
}
