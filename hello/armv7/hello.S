/*
 * hello.S
 * Copyright (C) 2014  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
main:	mov	r2, #strend - str - 1	/* string length */
	ldr	r1, =str		/* string address */
	mov	r0, #1			/* file descriptor 1 = stdout */
	mov	r7, #4			/* syscall 'write' (4) */
	svc	#0			/* call system */

	mov	r0, #0			/* return value */
	mov	r7, #1			/* sycall 'exit' (1) */
	svc	#0

	.data
str:
.string "Hello World!\n"
strend:
