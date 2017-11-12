/*
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#ifndef CELL__H
#define CELL__H

#include <assert.h>
#include <cstddef>
#include <cstdint>

enum cell_state { CELL_STATE_DEAD, CELL_STATE_LIFE };

class Cell {
public:
	Cell(enum cell_state state=CELL_STATE_DEAD);
	enum cell_state state(void) { return _state; }
	enum cell_state next_state(void) { return _next_state; }
	void set_state(enum cell_state s) { _state = s; }
	enum cell_state toggle_state(void);
	uint8_t life_neighbors(void) { return _life_neighbors; }
	void set_life_neighbors(size_t n) { assert(n <= 8); _life_neighbors = n; }
	unsigned int sim_step(void);
	void do_step(void) { _state = _next_state; };

private:
	enum cell_state _state; // State of cell in the current simulation step
	enum cell_state _next_state; // State of cell in the next simulation step
	uint8_t _life_neighbors; // Number of living neighbor cells
};

#endif // CELL__H
