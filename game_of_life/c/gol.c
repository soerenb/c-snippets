/*
 * gol.c - Conway's Game of Life
 * Copyright (C) 2014  Sören Brinkmann <soeren.brinkmann@gmail.com>
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
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <gopt.h>

enum cell_state {
	CELL_STATE_DEAD,
	CELL_STATE_LIFE
};

/**
 * struct cell:
 * @state:		State of cell in the current simulation step
 * @state_next:		State of cell in the next simulation step
 * @life_neighbors:	Number of living neighbor cells
 */
struct cell {
	enum cell_state state;
	enum cell_state next_state;
	uint8_t life_neighbors;
};

/**
 * struct world:
 * @cells:		  Pointer to cells in the world
 * @num_life_cells:	  Number living cells in the current simulation state
 * @num_life_cells_next:  Number living cells in the next simulation state
 * @height:		  World height
 * @width:		  World width
 * @wrap:		  Wrap world at its limits if true
 */
struct world {
	struct cell *cells;
	uintmax_t iteration;
	uintmax_t num_life_cells;
	uintmax_t num_life_cells_next;
	size_t height;
	size_t width;
	bool wrap;
};

/**
 * print_world()
 * @w:		Handle to struct world
 * @ch_life:	Character to draw living cells
 * @ch_dead:	Character to draw dead cells
 *
 * Print the current state of @w.
 */
static void print_world(struct world *w, const char *ch_life,
		const char *ch_dead)
{
	if (!ch_dead)
		ch_dead = " ";

	if (!ch_life)
		ch_life = "☺";

	for (uintptr_t y = 0; y < w->height; y++) {
		for (uintptr_t x = 0; x < w->width; x++) {
			struct cell *cell = w->cells + y * w->width + x;

			if (cell->state == CELL_STATE_DEAD) {
				if (cell->next_state == CELL_STATE_LIFE)
					attron(COLOR_PAIR(3));
				printw("%s", ch_dead);
			} else {
				if (cell->next_state == CELL_STATE_LIFE)
					attron(COLOR_PAIR(2));
				else
					attron(COLOR_PAIR(1));
				printw("%s", ch_life);
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
 * get_num_life_neighbors()
 * @w:	Handle to struct world
 * @_x:	Cell's x-coordinate
 * @_y:	Cell's y-coordinate
 *
 * Calculate the number of living neighbors for the cell located at @_x, @_y.
 *
 * Return: Number of living neighbor cells.
 */
static uintptr_t get_num_life_neighbors(struct world *w, size_t _x, size_t _y)
{
	size_t start_x, start_y, end_x, end_y;
	uintmax_t life_neighbors = 0;

	if (w->wrap) {
		start_y = _y == 0 ? w->height - 1 : _y - 1;
		start_x = _x == 0 ? w->width - 1 : _x - 1;
		end_y = (_y + 2) % w->height;
		end_x = (_x + 2) % w->width;
	} else {
		start_y = _y == 0 ? 0 : _y - 1;
		start_x = _x == 0 ? 0 : _x - 1;
		end_y = _y == w->height - 1 ? 0 : (_y + 2) % w->height;
		end_x = _x == w->width - 1 ? 0 : (_x + 2) % w->width;
	}

	for (size_t y = start_y; y != end_y; y = (y + 1) % w->height) {
		for (size_t x = start_x; x != end_x; x = (x + 1) % w->width) {
			if (w->cells[y * w->width + x].state ==
				       CELL_STATE_LIFE && !(x == _x && y == _y))
				life_neighbors++;
		}
	}

	return life_neighbors;
}

/**
 * toggle_cell_state()
 * @cell:	Handle to cell whose state to toggle
 *
 * Return:	New cell state
 */
static enum cell_state toggle_cell_state(struct cell *cell)
{
	if (cell->state == CELL_STATE_LIFE)
		cell->state = CELL_STATE_DEAD;
	else
		cell->state = CELL_STATE_LIFE;

	return cell->state;
}

/**
 * edit_world()
 * @w:	Handle to struct world
 *
 * Interactive world initialization.
 *
 * Return:	Number of living cells in the world.
 */
static uintmax_t edit_world(struct world *w)
{
	size_t x, y, ref_x, ref_y;
	bool edit = true;
	uintmax_t life_cells = 0;
	/* setup curses for editing */
	int tout = stdscr->_delay;	/* store current timeout */
	int visibility = curs_set(2);	/* cursor visiblity, 2 = very visible */

	timeout(-1);

	printw("World Editor: User arrow keys to move cursor, space to toggle cell state, q to exit editor mode.\n");

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
			state = toggle_cell_state(&w->cells[(y - ref_y) * w->width + (x - ref_x)]);
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
			if (x)
				x--;
			else
				x = w->width - 1;
			break;
		case 'l':
		case KEY_RIGHT:
			if ((x - ref_x) < w->width - 1)
				x++;
			else
				x = 0;
			break;
		case 'k':
		case KEY_UP:
			if (y > 1)
				y--;
			else
				y = ref_y + w->height - 1;
			break;
		case 'j':
		case KEY_DOWN:
			if ((y - ref_y) < w->height - 1)
				y++;
			else
				y = ref_y;
			break;
		case '$':
		case KEY_END:
			x = w->width - 1;
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
	timeout(tout);
	erase();

	return life_cells;
}


/**
 * create_world()
 * @height:	World height
 * @width:	World width
 * @wrap:	Wrap world at its limits
 * @edit:	Start world editor instead of random initialization
 *
 * Allocates a world with the size defined by @height and @width.
 *
 * Return:	Handle to the allocated struct world.
 */
static struct world *create_world(size_t height, size_t width, bool wrap,
				  bool edit)
{
	struct world *w;
	uintmax_t life_cells = 0;

	w = malloc(sizeof(*w));
	if (!w)
		return w;

	w->cells = malloc(height * width * sizeof(*w->cells));
	if (!w->cells) {
		free(w);
		return NULL;
	}

	w->iteration = 0;
	w->height = height;
	w->width = width;
	w->wrap = wrap;

	if (edit) {
		life_cells = edit_world(w);
	} else {
		for (size_t y = 0; y < height; y++) {
			for (size_t x = 0; x < width; x++) {
				uintptr_t rnd = random() % 15;

				if (rnd > 7) {
					w->cells[y * w->width + x].state =
						CELL_STATE_LIFE;
					life_cells++;
				} else {
					w->cells[y * w->width + x].state =
						CELL_STATE_DEAD;
				}
			}
		}
	}

	w->num_life_cells = life_cells;

	return w;
}

/**
 * destroy_world()
 * @w:	Handle to struct world
 *
 * Free all resources associated with @w.
 */
static void destroy_world(struct world *w)
{
	free(w->cells);
	free(w);
}

/**
 * simulate_step()
 * @w:	Handle to struct world
 *
 * Calculate the state of each cell in the next simulation state and the total
 * number of living cells in the next state.
 */
static void simulate_step(struct world *w)
{
	uintmax_t num_life_cells = 0;

	for (size_t y = 0; y < w->height; y++) {
		for (size_t x = 0; x < w->width; x++) {
			struct cell *cell = w->cells + y * w->width + x;
			enum cell_state state = cell->state;

			cell->life_neighbors = get_num_life_neighbors(w, x, y);

			if (state == CELL_STATE_DEAD) {
				if (cell->life_neighbors == 3) {
					cell->next_state = CELL_STATE_LIFE;
					num_life_cells++;
				} else {
					cell->next_state = CELL_STATE_DEAD;
				}
			} else {
				if (cell->life_neighbors < 2 ||
						cell->life_neighbors > 3) {
					cell->next_state = CELL_STATE_DEAD;
				} else {
					cell->next_state = CELL_STATE_LIFE;
					num_life_cells++;
				}
			}
		}
	}
	w->num_life_cells_next = num_life_cells;
}

/**
 * do_state_transition()
 * @w:	Handle to struct world
 *
 * Execute the transition to the next simulation state.
 */
static void do_state_transition(struct world *w)
{
	for (size_t y = 0; y < w->height; y++) {
		for (size_t x = 0; x < w->width; x++) {
			w->cells[y * w->width + x].state =
				w->cells[y * w->width + x].next_state;
			w->num_life_cells = w->num_life_cells_next;
		}
	}
}

/**
 * print_help() - print usage information
 * @str:	Binary name
 */
static void print_help(const char *str)
{
	printf("Usage:\n");
	printf("  %s [options]\n", str);
	printf("\n");
	printf("Options:\n");
	printf("  --help,   -h:   Print this help text and exit\n");
	printf("  --seed,   -s:   Seed for the random number generator\n");
	printf("  --delay,  -d:   Delay between simulation steps in ms\n");
	printf("  --manual, -m:   Advance simulation manually\n");
	printf("  --height, -g:   World height\n");
	printf("  --width,  -w:   World width\n");
	printf("  --edit,   -e:   Manually edit the initial state\n");
	printf("  --wrap,   -r:   Wrap world at its limits\n");
	printf("  --cell-dead :   Character to drawy dead cells\n");
	printf("  --cell-life :   Character to drawy living cells\n");
}

int main(int argc, const char *argv[])
{
	char ch;
	struct world *world;
	const char *arg_str;
	const char *ch_cell_life = NULL;
	const char *ch_cell_dead = NULL;
	bool edit = false;
	bool wrap = false;
	bool manual_step = false;
	size_t height = 0;
	size_t width = 0;
	uintptr_t seed = 0xdeadbeef;
	uintptr_t delay = 500;

	void *options = gopt_sort(&argc, argv, gopt_start(
				gopt_option('h', 0, gopt_shorts('h'),
					gopt_longs("help")),
				gopt_option('s', GOPT_ARG, gopt_shorts('s'),
					gopt_longs("seed")),
				gopt_option('e', 0, gopt_shorts('e'),
					gopt_longs("edit")),
				gopt_option('r', 0, gopt_shorts('r'),
					gopt_longs("wrap")),
				gopt_option('d', GOPT_ARG, gopt_shorts('d'),
					gopt_longs("delay")),
				gopt_option('m', 0, gopt_shorts('m'),
					gopt_longs("manual")),
				gopt_option('g', GOPT_ARG, gopt_shorts('g'),
					gopt_longs("height")),
				gopt_option('w', GOPT_ARG, gopt_shorts('w'),
					gopt_longs("width")),
				gopt_option('x', GOPT_ARG, NULL,
					gopt_longs("cell-dead")),
				gopt_option('y', GOPT_ARG, NULL,
					gopt_longs("cell-life"))
	));

	/* print help text and exit if help option is given */
	if (gopt(options, 'h')) {
		print_help(argv[0]);
		return 0;
	}

	if (gopt(options, 'm'))
		manual_step = true;

	if (gopt(options, 'e'))
		edit = true;

	if (gopt(options, 'r'))
		wrap = true;

	if (gopt_arg(options, 'd', &arg_str))
		delay = strtoul(arg_str, NULL, 0);

	if (gopt_arg(options, 's', &arg_str))
		seed = strtoul(arg_str, NULL, 0);

	if (gopt_arg(options, 'g', &arg_str)) {
		height = strtol(arg_str, NULL, 0);
		if (!height)
			fprintf(stderr, "WARN: invalid height\n");
	}

	if (gopt_arg(options, 'w', &arg_str)) {
		width = strtol(arg_str, NULL, 0);
		if (!width)
			fprintf(stderr, "WARN: invalid width\n");
	}

	gopt_arg(options, 'x', &ch_cell_dead);
	gopt_arg(options, 'y', &ch_cell_life);

	gopt_free(options);

	/* init random */
	srandom(seed);
	/* init curses */
	setlocale(LC_ALL, "");		/* needed for UTF-8 chars */
	initscr();			/* initialize screen */
	start_color();			/* enable colors */
	noecho();			/* no echoing of input by getch() */
	cbreak();			/* disable line buffering */
	nonl();
	keypad(stdscr, TRUE);		/* enable keypad */
	curs_set(0);			/* cursor visiblity, 0 = invisible */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	nodelay(stdscr, TRUE);
	if (manual_step)
		/* wait indefinitely for input in manual mode */
		timeout(-1);		/* blocking read */
	else
		timeout(delay);		/* non-blocking read with timeout */

	if (!width)
		width = getmaxx(stdscr) - 1;
	if (!height)
		height = getmaxy(stdscr) - 2;

	/* init the simulation */
	world = create_world(height, width, wrap, edit);
	if (!world)
		goto fail;

	while (world->num_life_cells) {
		printw("Iteration #%" PRIuMAX " Life Cells: %" PRIuMAX "",
		       world->iteration++, world->num_life_cells);

		simulate_step(world);
		print_world(world, ch_cell_life, ch_cell_dead);

		ch = getch();
		if (ch == 'q')
			break;

		do_state_transition(world);

		erase();
	}

	timeout(-1);
	printw("%" PRIuMAX " iterations have been simulated. ", world->iteration);
	if (world->num_life_cells)
		printw("%" PRIuMAX " cells are still alive.\n", world->num_life_cells);
	else
		printw("All life ceased to exist\n");
	refresh();
	destroy_world(world);
	getch();

fail:
	return endwin();
}
