/*
 * perf_numbers.cpp - Program finding perfect numbers
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
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[])
{
	unsigned int summe;
	unsigned int upperbound = 1000;
	unsigned int lowerbound = 1;
	unsigned int divcount = 0;
	unsigned int divs[100] = { 0 };

	double start = 0;
	double stop = 0;

	cout << "Searching perfect numbers:" << endl;
	cout << "Enter lower bound for search: ";
	cin >> lowerbound;
	cout << "Enter upper bound for search: ";
	cin >> upperbound;
	cout << endl << "Results: " << endl;

	start = omp_get_wtime();

	#pragma omp parallel for
	for (unsigned int i = lowerbound; i <= upperbound; i++) {
		summe = 0;
		divcount = 0;

		for (int j = (i >> 1); j > 0; j--) {
			if ((i % j) == 0) {
				summe += j;
				divs[divcount++] = j;
			}

			if (summe > i)
				break;
		}

		if (summe == i) {
			cout << i << "\t\tDivs: ";
			for (unsigned int j = 0; j < divcount; j++)
				cout << divs[j] << " ";
			cout << endl;
		}
	}

	stop = omp_get_wtime();

	cout << "Computation time = " << stop - start << endl;
	return 0;
}
