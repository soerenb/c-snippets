/*
 * vargs.c - Show usage of variable argument lists
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
#include <stdarg.h>

/**
 * avg() - Calculate arithmetic average
 * @fmt		Format string
 * @va_args	Format arguments
 * Returns average
 *
 * Valid character for the format string are:
 * 'i':		integer argument
 * 'd', 'f':	double argument
 * 'u':		unsigned int argument
 */
static double avg(char *fmt, ...)
{
	char ch;
	va_list args;
	int argint;
	double argdbl;
	unsigned int argu;
	double sum = 0;
	unsigned int argc = 0;

	va_start(args, fmt);

	while ((ch = *fmt)) {
		argc++;
		switch (ch) {
		case 'i':
			argint = va_arg(args, int);
			sum += (double)argint;
			break;
		case 'd':
		case 'f':
			argdbl = va_arg(args, double);
			sum += argdbl;
			break;
		case 'u':
			argu = va_arg(args, unsigned int);
			sum += (double)argu;
			break;
		default:
			argc--;
		}
		fmt++;
	}

	va_end(args);

	return sum / (double)argc;
}

int main(int argc, char *argv[])
{
	double foo;

	foo = avg("fifiiiiii", 1.0, 2, 3.0, 4, 5, 6, 7, 8, 9);
	printf("average: %f\n", foo);

	return 0;
}
