/*
 * memlayout.c - Illustrate how objects are placed in memory
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
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

struct a_struct {
	uint8_t		uint8;
	uint16_t	uint16;
	uint8_t		uint8_2;
	uint32_t	uint32;
	uint32_t	*ptr32;
};

int main(int argc, char *argv[])
{
	struct a_struct mystruct;
	size_t size;
	size_t padding;
	size_t size_acc = 0;
	size_t padding_acc = 0;
	uint32_t foo = 0xdeadbeef;
	uint8_t *ptr8 = (uint8_t *)&foo;
	uint16_t *ptr16 = (uint16_t *)&foo;

	printf("(uint32_t *): %p->%#8.8" PRIx32 "\n", &foo, foo);
	for (uintptr_t i = 0; i < sizeof(foo) / sizeof(*ptr16); i++, ptr16++)
		printf("(uint16_t *): %p->%#2.2" PRIx16 "\n", ptr16, *ptr16);

	for (uintptr_t i = 0; i < sizeof(foo) / sizeof(*ptr8); i++, ptr8++)
		printf("(uint8_t *):  %p->%#2.2" PRIx8 "\n", ptr8, *ptr8);
	printf("\n");


	printf("mystruct@%p\n", &mystruct);

	size = sizeof(mystruct.uint8);
	size_acc += size;
	printf("mystruct.uint8@%p, size:%zu\n", &mystruct.uint8, size);

	padding = __builtin_offsetof(typeof(mystruct), uint16) - size_acc -
			padding_acc;
	if (padding) {
		printf("padding:%zu\n", padding);
		padding_acc += padding;
	}

	size = sizeof(mystruct.uint16);
	size_acc += size;
	printf("mystruct.uint16@%p, size:%zu\n", &mystruct.uint16, size);

	padding = __builtin_offsetof(typeof(mystruct), uint8_2) - size_acc -
			padding_acc;
	if (padding) {
		printf("padding:%zu\n", padding);
		padding_acc += padding;
	}

	size = sizeof(mystruct.uint8_2);
	size_acc += size;
	printf("mystruct.uint8_2@%p, size:%zu\n", &mystruct.uint8_2, size);

	padding = __builtin_offsetof(typeof(mystruct), uint32) - size_acc -
			padding_acc;
	if (padding) {
		printf("padding:%zu\n", padding);
		padding_acc += padding;
	}

	size = sizeof(mystruct.uint32);
	size_acc += size;
	printf("mystruct.uint32@%p, size:%zu\n", &mystruct.uint32, size);

	padding = __builtin_offsetof(typeof(mystruct), ptr32) - size_acc -
			padding_acc;
	if (padding) {
		printf("padding:%zu\n", padding);
		padding_acc += padding;
	}

	size = sizeof(mystruct.ptr32);
	size_acc += size;
	printf("mystruct.ptr32@%p, size:%zu\n", &mystruct.ptr32, size);

	printf("mystruct: size:%zu, size payload:%zu, size padding:%zu\n",
			sizeof(mystruct), size_acc, padding_acc);

	return 0;
}
