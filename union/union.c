/*
 * union.c - Show union usage
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
#include <stdint.h>

union foo {
	uint32_t int32;
	uint16_t int16;
	uint8_t int8;
};

int main(int argc, char **argv)
{
	union foo myUnion;

	myUnion.int32 = 0xdeadbeef;

	printf("int32: %#x\n", myUnion.int32);
	printf("int16: %#x\n", myUnion.int16);
	printf("int8 : %#x\n", myUnion.int8);

	return 0;
}
