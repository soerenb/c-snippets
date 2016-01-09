/*
 * sizeof.c - Just print the size of common data types
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

int main(int argc, char *argv[])
{
	printf("sizeof(char): %zu\n", sizeof(char));
	printf("sizeof(short): %zu\n", sizeof(short));
	printf("sizeof(int): %zu\n", sizeof(int));
	printf("sizeof(long): %zu\n", sizeof(long));
	printf("sizeof(long long): %zu\n", sizeof(long long));
	printf("sizeof(float): %zu\n", sizeof(float));
	printf("sizeof(double): %zu\n", sizeof(double));
	printf("sizeof(int *): %zu\n", sizeof(int *));

	return 0;
}
