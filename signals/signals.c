/*
 * signals.c - Intercept signals
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
#include <signal.h>

static void sighndl(int sig)
{
	printf("Caught signal #%d\n", sig);
}

int main(int argc, char *argv[])
{
	int ch;
	struct sigaction sa;

	sa.sa_handler = sighndl;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);

	printf("Intercepting 'SIGINT'. Press 'q' to exit.\n");

	do {
		ch = getchar();
	} while (ch != 'q');

	return 0;
}
