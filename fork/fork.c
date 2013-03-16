/*
 * fork.c - Demonstrate fork and pipe operations
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
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <foo.h>

static int parent(int *pipefd, int cpid)
{
	char *input;
	unsigned int txBuf;
	
	close(pipefd[0]);
	while (1) {
		printf("> ");
		input = foo_gets();
		if (input == NULL) {
			printf("ERROR: Error while reading from stdin.\n");
			return -1;
		}
		if (!strcmp(input, "exit\n")) {
			close(pipefd[1]);
			kill(cpid, SIGKILL);
			return 0;
		}
		errno = 0;
		txBuf = strtol(input, NULL, 0);
		if (errno == ERANGE)
			printf("WARNING: Over-/Underflow in string conversion\
					to int.\n");
		free(input);
		if (write(pipefd[1], &txBuf, 4) != 4)
			printf("WARNING: Not all data was written.\n");
	}
}

static void child(int *pipefd)
{
	unsigned int rxBuf;
	
	close(pipefd[1]);
	while (1) {
		if (read(pipefd[0], &rxBuf, 4) != 4)
			printf("WARNING: Unexpected lengh in read data.\n");
		printf("%#8.8x\n", rxBuf);
	}
}

int main(int argc, char *argv[])
{
	pid_t cpid;
	int pipefd[2];
	int ret = 0;

	if (pipe(pipefd)) {
		printf("ERROR: Cannot open pipe.\n");
		return -1;
	}

	cpid = fork();
	if (cpid == -1) {
		printf("ERROR: Cannot fork.\n");
		return -1;
	}

	if (cpid)
		ret = parent(pipefd, cpid);
	else
		child(pipefd);

	return ret;
}
