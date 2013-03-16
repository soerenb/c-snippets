/*
 * hello.S
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
	.global main
	.global _start

	.text
_start:
main:	movq	$(strend - str - 1), %rdx	/* string length */
	lea	str, %rsi			/* string address */
	movq	$1, %rdi			/* file descriptor */
	movq	$1, %rax			/* syscall 'write' */
	syscall					/* call system */

	movq	$0, %rdi			/* return value */
	movq	$60, %rax			/* sycall 'exit' */
	syscall

	.data
str:
.string "Hello World!\n\0"
strend:
