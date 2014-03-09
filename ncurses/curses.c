/*
 * curses.c - ncurses hello world
 * Copyright (C) 2013 - 2014  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <ncurses.h>

#define MAXCH	25

int main(int argc, char *argv[])
{
	int ch;
	int keep_running = 1;
	unsigned int ch_cnt = 0;

	initscr();
	noecho();

	attron(A_BLINK);
	printw("hello world!\n");
	attroff(A_BLINK);

	while (keep_running) {
		refresh();
		ch = getchar();
		if (ch == 'q' || ch == 'Q')
			keep_running = 0;
		if (ch == '\r')
			printw("\n");
		printw("%c", (char)ch);

		if (++ch_cnt == MAXCH) {
			erase();
			ch_cnt = 0;
		}
	}

	return endwin();
}
