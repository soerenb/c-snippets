/*
 * revstr.c - Reverse a string
 * Copyright (C) 2015-2016  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <string.h>
#include <foo.h>

/**
 * revstr_new - Reverse string
 * @s:	String to reverse
 * Return: The reversed string as newly allocated object or NULL. The input
 *	   string is not modified. The caller is responsible for freeing the
 *	   returned object.
 */
static char *revstr_new(const char *s)
{
	if (!s)
		return NULL;

	size_t l = strlen(s);
	char *r = malloc(l + 1);
	r[l] = '\0';
	for (size_t i = 0; i < l; i++)
		r[i] = s[l - i - 1];

	return r;
}

/**
 * revstr_inplace - Reverse string
 * @s:	String to reverse
 *
 * The string @s is reversed in place.
 */
static void revstr_inplace(char *s)
{
	if (!s)
		return;

	size_t l = strlen(s);
	for (size_t i = 0; i < l / 2; i++) {
		char c = s[i];

		s[i] = s[l - i - 1];
		s[l - i - 1] = c;
	}
}

int main(int argc, char *argv[])
{
	char *string;

	if (argc > 1) {
		string = argv[1];
	} else {
		printf("Enter string: ");
		string = foo_gets();
		if (string[strlen(string) - 1] == '\n')
			string[strlen(string) - 1] = '\0';
	}

	char *rev = revstr_new(string);

	printf("in:   %s\n", string);
	printf("out1: %s\n", rev);
	revstr_inplace(string);

	if (strcmp(string, rev))
		fprintf(stderr, "ERROR: results differ\n");
	printf("out2: %s\n", string);

	return 0;
}
