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

	.data
_start:
main:
	adr	r6, 1f		/* r6 = &arg */
	str	r6, [r6, #8]	/* argv[0] = /bin/sh */
	eor	r3, r3
	str	r3, [r6, #12]	/* argv[1] = NULL */
	add	r1, r6, #8	/* r1 = &argv */
	add	r2, r6, #12	/* r2 = envp */
	mov	r7, #11		/* syscall 'execve' (11) */
	mov	r0, r6		/* r0 = arg */
	svc	#0		/* call system */

	eor	r0, r0		/* return value */
	mov	r7, #1		/* sycall 'exit' (1) */
	svc	#0

1:
	.string "/bin/sh\0"
