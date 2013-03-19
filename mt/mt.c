/*
 * mt.c - Experiment with multiple threads
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
#include <unistd.h>
#include <pthread.h>

/* Constants */
static const int THREADC = 10;

/* Global vars */
static unsigned int counter;
static unsigned int counter2;

/* Mutex */
static pthread_mutex_t mutex_counter;

/* First thread version manipulating both counters w/o using mutex */
static void *thread(void *arg)
{
#ifdef VERBOSE
	unsigned int id = *((unsigned int *)arg);
	printf("Hello from thread %u! Counter: %u\n", id, counter);
#endif

	counter2 += counter;
	sleep(random() % 2);
	counter++;

	pthread_exit(NULL);
}

/* Second thread version manipulating both counters using mutex */
static void *thread_safe(void *arg)
{
#ifdef VERBOSE
	unsigned int id = *((unsigned int *)arg);
	printf("Hello from thread %u! Counter: %u\n", id, counter);
#endif

	/* Lock mutex -> compute -> release mutex */
	pthread_mutex_lock(&mutex_counter);

	counter2 += counter;
	sleep(random() % 2);
	counter++;

	pthread_mutex_unlock(&mutex_counter);

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i;
	pthread_t threads[THREADC];
	pthread_attr_t attr[THREADC];
	int tret;
	unsigned int id[THREADC];
	unsigned int ref_counter = 0;

	pthread_mutex_init(&mutex_counter, NULL);

	/* Spawn unsafe threads */
	for (i = 0; i < THREADC; i++) {
		/* Initialize pthread_attr objects */
		pthread_attr_init(&attr[i]);
		/* Assure threads are joinable */
		pthread_attr_setdetachstate(&attr[i], PTHREAD_CREATE_JOINABLE);

		/* Assign unique ID */
		id[i] = i;
		/* Actual thread spawning */
		tret = pthread_create(&threads[i], &attr[i], &thread,
				(void *)(&id[i]));
		if (tret)
			printf("error %d while spawning thread\n", tret);
	}

	/* Join previously with spawned threads */
	for (i = 0; i < THREADC; i++) {
		ref_counter += i; /* referece value to compare counter 2 with */
		pthread_join(threads[i], NULL);	/* actual joining */
	}

	/* Print out counter values */
	printf("Multithreaded w/o mutex:\n");
	printf("Counter:    %u\n", counter);
	printf("Counter2:   %u\n", counter2);
	printf("ref_counter: %u\n", ref_counter);
	printf("THREADC:    %u\n", THREADC);

	/* Reset counter */
	counter = 0;
	counter2 = 0;

	/* Spawn safe threads */
	for (i = 0; i < THREADC; i++)
		tret = pthread_create(&threads[i], &attr[i], &thread_safe,
				(void *)(&id[i]));
		if (tret)
			printf("error %d while spawning thread\n", tret);

	/* Join threads */
	for (i = 0; i < THREADC; i++)
		pthread_join(threads[i], NULL);

	/* Print out counter values */
	printf("Multithreaded w/ mutex:\n");
	printf("Counter:    %u\n", counter);
	printf("Counter2:   %u\n", counter2);
	printf("ref_counter: %u\n", ref_counter);
	printf("THREADC:    %u\n", THREADC);

	return 0;
}
