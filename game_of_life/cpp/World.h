/*
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#ifndef WORLD__H
#define WORLD__H

#include "Cell.h"
#include <cstddef>
#include <string>

class World {
public:
	World(size_t height, size_t width, bool wrap=false, bool edit=false);
	~World(void);
	void simulate_step(void);
	void do_state_transition(void);
	void print(std::string ch_life="☺", std::string ch_dead=" ");
	uintmax_t life_cells(void) { return _num_life_cells; }
	uintmax_t iteration(void) { return _iteration; }

private:
	Cell *_cells; // Pointer to cells in the world
	uintmax_t _iteration;
	uintmax_t _num_life_cells; // Number living cells in the current simulation state
	uintmax_t _num_life_cells_next; // Number living cells in the next simulation state
	size_t _height;
	size_t _width;
	bool _wrap; // Wrap world at its limits if true

	uintmax_t _edit(void);
	uintmax_t _life_neighbors(size_t _x, size_t _y);
};

#endif // WORLD__H
