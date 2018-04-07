/*
 * fam.c - Flexible array member snippet
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
#include <stddef.h>

#define SIZE	10

struct foo {
	size_t size;
	unsigned int an_arr[];
};

int main(int argc, char *argv[])
{
	struct foo *bar = malloc(offsetof(struct foo, an_arr) +
				 SIZE * sizeof(bar->an_arr[0]));
	if (!bar) {
		return -1;
	}
	bar->size = SIZE;

	for (size_t i = 0; i < bar->size; i++) {
		bar->an_arr[i] = 0xbabe << 16 | i;
	}

	printf("sizeof(struct foo):%zu\n", sizeof(struct foo));
	printf("sizeof(struct foo->an_arr[0]):%zu\n", sizeof(((struct foo *)0)->an_arr[0]));
	printf("sizeof(*bar):%zu\n", sizeof(*bar));

	for (size_t i = 0; i < bar->size; i++) {
		printf("bar->an_arr[%zu]:%#x\n", i, bar->an_arr[i]);
	}

	free(bar);

	return 0;
}
