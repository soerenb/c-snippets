/*
 * server.c - Project to demonstrate IPC via UNIX Domain Sockets, server part
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
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "interface.h"

#define BUFSIZE	256

/**
 * echo_srv - Echo server connection handler
 * @arg	Connection file descriptor
 * Returns always NULL
 */
static void *echo_srv(void *arg)
{
	int fd = (int)((long long)arg);
	char buf[BUFSIZE];

	while (1) {
		intptr_t bytes_received = read(fd, &buf, BUFSIZE);
		if (bytes_received < 0) {
			perror("ERROR: unable to receive data");
			continue;
		} else {
			printf("SERVER: received %" PRIdPTR " bytes\n",
			       bytes_received);
		}

		errno = 0;
		intptr_t bytes_sent = write(fd, buf, bytes_received);
		if (bytes_sent < 0) {
			perror("ERROR: unable to send data");
			if (errno == EPIPE)
				break;

		} else {
			printf("SERVER: sent %" PRIdPTR " bytes\n", bytes_sent);
		}
	}

	close(fd);

	return NULL;
}

int main(void)
{
	int ret;
	pthread_t thread;
	pthread_attr_t thread_attr;
	struct sockaddr_un address;
	socklen_t address_length = sizeof(struct sockaddr_un);

	/* ignore SIGPIPE */
	signal(SIGPIPE, SIG_IGN);

	/* create socket */
	int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		perror("ERROR: unable to create socket\n");
		return socket_fd;
	}

	/* configure socket address */
	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, SERVER_SOCK_FILE);

	/* bind socket to address */
	ret = bind(socket_fd, (const struct sockaddr *)&address,
			sizeof(address));
	if (ret < 0) {
		close(socket_fd);
		perror("ERROR: unable to bind to socket");
		goto err_close;
	}

	/* listen for connection attemps */
	ret = listen(socket_fd, 5);
	if (ret) {
		printf("ERROR: listen failed\n");
		goto err_unlink;
	}

	int connection_fd = accept(socket_fd, (struct sockaddr *)&address,
			&address_length);
	if (connection_fd < 0) {
		perror("ERROR: accept failed");
		ret = connection_fd;
		goto err_unlink;
	}

	/* Initialize pthread_attr objects */
	pthread_attr_init(&thread_attr);
	/* Assure threads are joinable */
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&thread, &thread_attr, echo_srv,
			(void *)((long long)connection_fd));
	if (ret) {
		printf("ERROR: unable to create thread\n");
		close(connection_fd);
		goto err_unlink;
	}

	pthread_join(thread, NULL);

err_unlink:
	unlink(SERVER_SOCK_FILE);
err_close:
	close(socket_fd);

	return ret;
}
