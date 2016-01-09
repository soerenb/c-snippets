/*
 * client.c - Project to demonstrate IPC via Linux Domain Sockets, client part
 * Copyright (C) 2013  Sören Brinkmann <soeren.brinkmann@gmail.com>
 * Based on the examples by Thomas Stover (http://www.thomasstover.com/uds.html)
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

#include <sys/socket.h>
#include <sys/un.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <foo.h>
#include "interface.h"

int main(void)
{
	int ret = 0;
	struct sockaddr_un address;

	int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		perror("ERROR: unable to create socket");
		return socket_fd;
	}

	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, SERVER_SOCK_FILE);

	if (connect(socket_fd, (const struct sockaddr *)&address,
				sizeof(address))) {
		perror("ERROR: unable to connect");
		ret = -1;
		goto err_close;
	}

	while (1) {
		printf("Enter message to transmit: ");
		char *buf = foo_gets();
		if (!buf) {
			printf("ERROR: unable to create message\n");
			ret = -1;
			goto err_close;
		}
		size_t buf_size = strlen(buf);

		if (!strncmp("quit", buf, 4) || !strncmp("exit", buf, 4)) {
			printf("bye\n");
			ret = 0;
			free(buf);
			goto err_close;
		}

		intptr_t bytes_sent = write(socket_fd, buf, buf_size - 1);
		if (bytes_sent < 0) {
			perror("ERROR: unable to send data");
			ret = bytes_sent;
			goto err_close;
		} else {
			printf("CLIENT: sent %" PRIdPTR " bytes\n", bytes_sent);
		}

		intptr_t bytes_received = read(socket_fd, buf, buf_size - 1);
		if (bytes_received < 0) {
			perror("ERROR: unable to receive data");
			ret = bytes_received;
			goto err_close;
		} else {
			printf("CLIENT: received %" PRIdPTR " bytes\n",
			       bytes_received);
		}

		buf[bytes_received] = '\0';
		printf("Received message: %s\n", buf);
		free(buf);
	}

err_close:
	close(socket_fd);

	return ret;
}
