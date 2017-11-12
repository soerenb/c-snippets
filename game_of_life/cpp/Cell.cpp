/*
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include "Cell.h"

Cell::Cell(enum cell_state state)
{
	_state = state;
}

/**
 * toggle_state()
 *
 * Return:	New cell state
 */
enum cell_state Cell::toggle_state(void)
{
	if (_state == CELL_STATE_LIFE) {
		_state = CELL_STATE_DEAD;
	} else {
		_state = CELL_STATE_LIFE;
	}

	return _state;
}

unsigned int Cell::sim_step(void)
{
	unsigned int ret = 0;

	if (_state == CELL_STATE_DEAD) {
		if (_life_neighbors == 3) {
			_next_state = CELL_STATE_LIFE;
			ret = 1;
		} else {
			_next_state = CELL_STATE_DEAD;
		}
	} else {
		if (_life_neighbors < 2 || _life_neighbors > 3) {
			_next_state = CELL_STATE_DEAD;
		} else {
			_next_state = CELL_STATE_LIFE;
			ret = 1;
		}
	}

	return ret;
}
