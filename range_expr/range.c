/*
 * range.c - Demonstrate range expressions
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
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	for (uintptr_t i = 0; i < 10; i++) {
		switch (i) {
		case 0 ... 3:
			printf("0 <= i <= 3\n");
			break;
		case 4 ... 6:
			printf("4 <= i <= 6\n");
			break;
		case 7 ... 9:
			printf("7 <= i <= 9\n");
			break;
		case 10:
			printf("i == 10\n");
		default:
			break;
		}
	}

	return 0;
}
