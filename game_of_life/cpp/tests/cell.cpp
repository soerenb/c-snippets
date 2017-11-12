/*
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <Cell.h>

static const enum cell_state dead2x[] {
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_LIFE,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
};

static const enum cell_state life2x[] {
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_LIFE,
	CELL_STATE_LIFE,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
	CELL_STATE_DEAD,
};

int main(int argc, char *argv[])
{
	// new cell with default state
	Cell *cell = new Cell;

	// test default state
	if (cell->state() != CELL_STATE_DEAD) {
		return 1;
	}

	// test set_state
	cell->set_state(CELL_STATE_LIFE);
	if (cell->state() != CELL_STATE_LIFE) {
		return 2;
	}

	delete cell;

	// test non-default constructor
	cell = new Cell(CELL_STATE_LIFE);
	if (cell->state() != CELL_STATE_LIFE) {
		return 3;
	}

	// test toggle state
	cell->toggle_state();
	if (cell->state() != CELL_STATE_DEAD) {
		return 4;
	}

	// test transitions from dead state
	for (size_t i = 0; i <= 8; i++) {
		cell->set_state(CELL_STATE_DEAD);
		cell->set_life_neighbors(i);

		cell->sim_step();
		if (cell->next_state() != dead2x[i]) {
			return 5 + (i << 8);
		}

		cell->do_step();
		if (cell->state() != dead2x[i]) {
			return 6 + (i << 16);
		}
	}

	// test transitions from life state
	for (size_t i = 0; i <= 8; i++) {
		cell->set_state(CELL_STATE_LIFE);
		cell->set_life_neighbors(i);

		cell->sim_step();
		if (cell->next_state() != life2x[i]) {
			return 7 + (i << 8);
		}

		cell->do_step();
		if (cell->state() != life2x[i]) {
			return 8 + (i << 16);
		}
	}

	return 0;
}
