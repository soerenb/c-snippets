/*
 * backtrace.c - Obtain a backtrace
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
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <execinfo.h>

static uintptr_t array[] = {
	99,
	1,
	37,
	23,
	42
};

/**
 * sighndl - Signal handler
 * @sig: Received signal
 *
 * This handler is invoked on segfaults. It obtains and prints a stacktrace
 * before exiting the program.
 */
void sighndl(int sig)
{
	void *buf[32];

	printf("*********** segfault ***********\n\n");

	size_t size = backtrace(buf, sizeof(buf));
	char **trace = backtrace_symbols(buf, size);

	printf("backtrace:\n");
	for (size_t i = 0; i < size; i++)
		fprintf(stderr, "%s\n", trace[i]);

	exit(-1);
}

void cmp2(void)
{
	/* cause a segfault */
	memcpy((void *)1, "foobar", 42);
}

int cmp1(const void *foo, const void *bar)
{
	cmp2();
	return 0;
}

void func2(void)
{
	qsort(array, sizeof(array) / sizeof(array[0]), sizeof(array[0]), cmp1);
}

void func1(void)
{
	func2();
}

int main(int argc, char *argv[])
{
	struct sigaction sa;

	/* register custom handler for segfaults */
	sa.sa_handler = sighndl;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGSEGV, &sa, NULL);

	func1();

	return 0;
}
