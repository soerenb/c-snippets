/*
 * stringify.c - Stringification snippet
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <stdio.h>

#define print_str(str)				\
	static void print_ ## str (void)	\
	{					\
		printf("%s\n", #str);		\
	}

print_str(foobar)

int main(int argc, char *argv[])
{
	print_foobar();

	return 0;
}
