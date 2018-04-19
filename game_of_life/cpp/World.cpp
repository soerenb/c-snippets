/*
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include "World.h"
#include <ncurses.h>

/**
 * print()
 * @ch_life:	Character to draw living cells
 * @ch_dead:	Character to draw dead cells
 *
 * Print the current world state.
 */
void World::print(std::string ch_life, std::string ch_dead)
{
	if (ch_dead.empty()) {
		ch_dead = " ";
	}
	if (ch_life.empty()) {
		ch_life = "☺";
	}
	for (size_t y = 0; y < _height; y++) {
		for (size_t x = 0; x < _width; x++) {
			Cell *cell = _cells + y * _width + x;

			if (cell->state() == CELL_STATE_DEAD) {
				if (cell->next_state() == CELL_STATE_LIFE) {
					attron(COLOR_PAIR(3));
				}
				printw("%s", ch_dead.c_str());
			} else {
				if (cell->next_state() == CELL_STATE_LIFE) {
					attron(COLOR_PAIR(2));
				} else {
					attron(COLOR_PAIR(1));
				}
				printw("%s", ch_life.c_str());
			}
			attroff(COLOR_PAIR(1));
			attroff(COLOR_PAIR(2));
			attroff(COLOR_PAIR(3));
		}
		printw("\n");
	}
	refresh();
}

/**
 * create_world()
 * @height:	World height
 * @width:	World width
 * @wrap:	Wrap world at its limits
 * @do_edit:	Start world editor instead of random initialization
 *
 * Return:	Handle to the allocated struct world.
 */
World::World(size_t height, size_t width, bool wrap, bool do_edit)
{
	uintmax_t life_cells = 0;

	_cells = new Cell[height * width * sizeof(*_cells)];

	_iteration = 0;
	_height = height;
	_width = width;
	_wrap = wrap;

	if (do_edit) {
		life_cells = _edit();
	} else {
		for (size_t y = 0; y < height; y++) {
			for (size_t x = 0; x < width; x++) {
				size_t rnd = random() % 15;

				if (rnd > 7) {
					_cells[y * _width + x].set_state(CELL_STATE_LIFE);
					life_cells++;
				} else {
					_cells[y * _width + x].set_state(CELL_STATE_DEAD);
				}
			}
		}
	}

	_num_life_cells = life_cells;
}

/**
 * destroy_world()
 *
 * Free all resources associated with world.
 */
World::~World(void)
{
	delete[] _cells;
}

/**
 * _life_neighbors()
 * @_x:	Cell's x-coordinate
 * @_y:	Cell's y-coordinate
 *
 * Calculate the number of living neighbors for the cell located at @_x, @_y.
 *
 * Return: Number of living neighbor cells.
 */
size_t World::_life_neighbors(size_t _x, size_t _y)
{
	size_t start_x, start_y, end_x, end_y;
	uintmax_t life_neighbors = 0;

	if (_wrap) {
		start_y = _y == 0 ? _height - 1 : _y - 1;
		start_x = _x == 0 ? _width - 1 : _x - 1;
		end_y = (_y + 2) % _height;
		end_x = (_x + 2) % _width;
	} else {
		start_y = _y == 0 ? 0 : _y - 1;
		start_x = _x == 0 ? 0 : _x - 1;
		end_y = _y == _height - 1 ? 0 : (_y + 2) % _height;
		end_x = _x == _width - 1 ? 0 : (_x + 2) % _width;
	}

	for (size_t y = start_y; y != end_y; y = (y + 1) % _height) {
		for (size_t x = start_x; x != end_x; x = (x + 1) % _width) {
			if (_cells[y * _width + x].state() ==
				CELL_STATE_LIFE &&
			    !(x == _x && y == _y)) {
				life_neighbors++;
			}
		}
	}

	return life_neighbors;
}

/**
 * _edit() - Interactive world initialization.
 *
 * Return:	Number of living cells in the world.
 */
uintmax_t World::_edit(void)
{
	size_t x, y, ref_x, ref_y;
	bool edit = true;
	uintmax_t life_cells = 0;
	/* setup curses for editing */
	int visibility = curs_set(2); /* cursor visiblity, 2 = very visible */

	printw(
	    "World Editor: User arrow keys to move cursor, space to toggle cell state, q to exit editor mode.\n");

	getyx(stdscr, ref_y, ref_x);
	x = ref_x;
	y = ref_y;
	while (edit) {
		int ch;
		enum cell_state state;

		ch = getch();
		switch (ch) {
		case 'q':
			edit = false;
			break;
		case ' ':
			state = _cells[(y - ref_y) * _width + (x - ref_x)].toggle_state();
			if (state == CELL_STATE_LIFE) {
				life_cells++;
				printw("☺");
			} else {
				life_cells--;
				printw(" ");
			}

			break;
		case 'h':
		case KEY_LEFT:
			if (x) {
				x--;
			} else {
				x = _width - 1;
			}
			break;
		case 'l':
		case KEY_RIGHT:
			if ((x - ref_x) < _width - 1) {
				x++;
			} else {
				x = 0;
			}
			break;
		case 'k':
		case KEY_UP:
			if (y > 1) {
				y--;
			} else {
				y = ref_y + _height - 1;
			}
			break;
		case 'j':
		case KEY_DOWN:
			if ((y - ref_y) < _height - 1) {
				y++;
			} else {
				y = ref_y;
			}
			break;
		case '$':
		case KEY_END:
			x = _width - 1;
			break;
		case '^':
		case KEY_HOME:
			x = 0;
			break;
		}
		move(y, x);
		refresh();
	}

	/* restore curses simulation settings */
	curs_set(visibility);
	erase();

	return life_cells;
}

/**
 * simulate_step()
 *
 * Calculate the state of each cell in the next simulation state and the total
 * number of living cells in the next state.
 */
void World::simulate_step(void)
{
	uintmax_t num_life_cells = 0;

	for (size_t y = 0; y < _height; y++) {
		for (size_t x = 0; x < _width; x++) {
			Cell *cell = _cells + y * _width + x;

			cell->set_life_neighbors(_life_neighbors(x, y));
			num_life_cells += cell->sim_step();
		}
	}
	_num_life_cells_next = num_life_cells;
}

/**
 * do_state_transition()
 *
 * Execute the transition to the next simulation state.
 */
void World::do_state_transition(void)
{
	for (size_t y = 0; y < _height; y++) {
		for (size_t x = 0; x < _width; x++) {
			_cells[y * _width + x].do_step();
		}
	}
	_num_life_cells = _num_life_cells_next;

	_iteration++;
}
