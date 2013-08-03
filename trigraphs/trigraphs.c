/*
 * trigraphs.c - Illustrate trigraphs
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

int main(int argc, char *argv??(??))
??<
	unsigned int a, b;

	printf("hello trigraphs??/n");

	a = 0xc;
	b = 5;
	a ??'= b;
	printf("a: %??=x??/n", a);
	a = ??-a;
	printf("a: %??=x??/n", a);
	a ??!= 1;
	printf("a: %??=x??/n", a);

	return 0;
??>
