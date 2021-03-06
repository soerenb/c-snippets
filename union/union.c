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
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

/* gives us the ability to access single bits in byte1 */
struct byte1_bitfields {
	uint8_t bit0 :1; /* Least siginificant bit */
	uint8_t bit1 :1;
	uint8_t bit2 :1;
	uint8_t bit3 :1;
	uint8_t bit4_7 :4; /* Most significant bits */
};

union bar {
	uint8_t allElements[4];

	struct {
		uint8_t byte0;
		union {
			uint8_t byte1;
			struct byte1_bitfields byte1_elements;
		};
		uint8_t bytes[2];
	} elements;
} __attribute__((packed));

union foo {
	uint32_t int32;
	uint16_t int16;
	uint8_t int8;
};

int main(int argc, char **argv)
{
	/* Foo tests */
	union foo my_union;

	my_union.int32 = 0xdeadbeef;

	printf("int32: %#" PRIx32 "\n", my_union.int32);
	printf("int16: %#" PRIx16 "\n", my_union.int16);
	printf("int8 : %#" PRIx8 "\n", my_union.int8);

	/* Bar tests */
	/* Accessing serialized bytes and/or bits */
	union bar my_bar;

	my_bar.allElements[0] = 0xAA;
	printf("byte0:\t %#02x\n", my_bar.elements.byte0);

	my_bar.elements.byte1 = 0x2E; /* equals 0b0010 1110 */
	printf("byte1-bit0:\t %#02x\n", my_bar.elements.byte1_elements.bit0);
	printf("byte1-bit1:\t %#02x\n", my_bar.elements.byte1_elements.bit1);
	printf("byte1-bit2:\t %#02x\n", my_bar.elements.byte1_elements.bit2);
	printf("byte1-bit3:\t %#02x\n", my_bar.elements.byte1_elements.bit3);
	printf("byte1-bits[4..7]: %#02x\n\n",
			my_bar.elements.byte1_elements.bit4_7);

	return 0;
}
