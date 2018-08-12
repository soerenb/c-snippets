/*
 * blocks.c - Blocks snippet
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

static void call_block(void (^x)(void))
{
	x();
}

static void call_block_with_arg(void (^x)(unsigned int))
{
	x(23);
}

static unsigned int call_block_ret(unsigned int (^x)(void))
{
	return x();
}

int main(int argc, char *argv[])
{
	unsigned int foo = 42;

	call_block(^{printf("%s: %s(%u): hello from block\n", __FILE__,
				__func__, __LINE__);});
	call_block_with_arg(^(unsigned int arg) {printf("block called with arg:%u\n", arg);});
	call_block(^{printf("block: foo:%u\n", foo);});

	unsigned int ret = call_block_ret(^ unsigned int (void) {return 4711;});
	printf("ret:%u\n", ret);

	return 0;
}
