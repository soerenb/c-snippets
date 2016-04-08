/*
 * N-Queen Problem - Position eight queens on a chessboard so they
 *		     don't share any row, colum or diagonal
 * Copyright (C) 2016  Sören Brinkmann <soeren.brinkmann@gmail.com>
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

#define NDEBUG
//#define PRINT_BOARD

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS	8
#define COLS	ROWS

struct coord {
	size_t x;
	size_t y;
};

struct {
	size_t wp;
	struct coord data[ROWS];
} stack;

static void push(size_t x, size_t y)
{
	assert(x < ROWS && y < COLS && stack.wp >= 0 && stack.wp < ROWS);

	stack.data[stack.wp].x = x;
	stack.data[stack.wp].y = y;
	stack.wp++;
}

static struct coord *pop(void)
{
	assert(stack.wp > 0 && stack.wp <= ROWS);

	stack.wp--;
	return &stack.data[stack.wp];
}

static size_t cols_taken[COLS];
static size_t diaglr_taken[(ROWS + COLS) - 1];
static size_t diagrl_taken[(ROWS + COLS) - 1];

static int coord2diaglr(size_t x, size_t y)
{
	return x + y;
}

static int coord2diagrl(size_t x, size_t y)
{
	return ROWS - 1 - (x - y);
}

static void coord_mark(size_t x, size_t y)
{
	assert(x < ROWS && y < COLS);

	cols_taken[x] = 1;
	diaglr_taken[coord2diaglr(x, y)] = 1;
	diagrl_taken[coord2diagrl(x, y)] = 1;
}

static void coord_unmark(size_t x, size_t y)
{
	assert(x < ROWS && y < COLS);

	cols_taken[x] = 0;
	diaglr_taken[coord2diaglr(x, y)] = 0;
	diagrl_taken[coord2diagrl(x, y)] = 0;
}

static void print_board(size_t position)
{
	printf("Position #%zu:\n", position);
	for (size_t i = 0; i < ROWS; i++) {
#ifdef PRINT_BOARD
		for (size_t j = 0; j < COLS; j++) {
			if (j == stack.data[i].x)
				printf("Q");
			else
				printf("_");
		}
		printf("\n");
#else
		printf("  x:%zu, y:%zu\n", stack.data[i].x, stack.data[i].y);
#endif
	}
}

static unsigned int coord_is_free(size_t x, size_t y)
{
	assert(x < ROWS && y < COLS);

	if (cols_taken[x])
		return 0;

	if (diaglr_taken[coord2diaglr(x, y)])
		return 0;

	if (diagrl_taken[coord2diagrl(x, y)])
		return 0;

	return 1;
}

static void do_find_pos(size_t row)
{
	assert(row < ROWS);

	static size_t position;

	for (size_t i = 0; i < COLS; i++) {
		/* check if this field is free */
		if (!coord_is_free(i, row))
			continue;

		/* valid position */
		push(i, row);

		/* mark off current field */
		coord_mark(i, row);

		if (row == ROWS - 1) {
			/* all queens are positioned -> print */
			print_board(++position);
		} else {
			/* position next queen */
			do_find_pos(row + 1);
		}

		/* discard position */
		struct coord *e = pop();
		coord_unmark(e->x, e->y);
	}
}

static void find_queen_positions(void)
{
	do_find_pos(0);
}

int main(int argc, char *argv[])
{
	find_queen_positions();

	return 0;
}
