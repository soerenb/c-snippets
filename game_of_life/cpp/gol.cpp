/*
 * gol.c - Conway's Game of Life
 * Copyright (C) 2014 - 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <iostream>
#include <inttypes.h>
#include <locale.h>
#include <ncurses.h>
#include <cxxopts.hpp>
#include "World.h"
#include "Cell.h"

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

int main(int argc, char *argv[])
{
	std::string ch_cell_life, ch_cell_dead;

	cxxopts::Options options("game_of_life", "Conway's Game of Life");
	options.add_options()
		("h,help", "Print help text")
		("s,seed", "Random seed for initialization", cxxopts::value<unsigned int>()->default_value("0xdeadbeef"))
		("e,edit", "Edit initial state")
		("r,wrap", "World wraps around")
		("d,delay", "Delay between simulation steps", cxxopts::value<unsigned long>()->default_value("500"))
		("m,manual", "Do not automatically advance simulation")
		("g,height", "World height", cxxopts::value<size_t>()->default_value("0"))
		("w,width", "World width", cxxopts::value<size_t>()->default_value("0"))
		("x,cell-dead", "Charactor to display dead cells", cxxopts::value<std::string>())
		("y,cell-life", "Charactor to display life cells", cxxopts::value<std::string>())
		;

	options.parse(argc, argv);

	/* print help text and exit if help option is given */
	if (options["h"].as<bool>()) {
		print_help(argv[0]);
		return 0;
	}

	bool manual_step = options["m"].as<bool>();
	bool edit = options["e"].as<bool>();
	bool wrap = options["r"].as<bool>();
	unsigned long delay = options["d"].as<unsigned long>();
	unsigned int seed = options["s"].as<unsigned int>();
	size_t height = options["g"].as<size_t>();
	size_t width = options["w"].as<size_t>();

	if (options.count("x")) {
		ch_cell_dead = options["x"].as<std::string>();
	}
	if (options.count("y")) {
		ch_cell_life = options["y"].as<std::string>();
	}

	/* init random */
	srandom(seed);
	/* init curses */
	setlocale(LC_ALL, "");	/* needed for UTF-8 chars */
	initscr();		/* initialize screen */
	start_color();		/* enable colors */
	noecho();		/* no echoing of input by getch() */
	cbreak();		/* disable line buffering */
	nonl();
	keypad(stdscr, TRUE);	/* enable keypad */
	curs_set(0);		/* cursor visiblity, 0 = invisible */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	nodelay(stdscr, TRUE);
	if (manual_step) {
		/* wait indefinitely for input in manual mode */
		timeout(-1); /* blocking read */
	} else {
		timeout(delay); /* non-blocking read with timeout */
	}

	if (!width) {
		width = getmaxx(stdscr) - 1;
	}
	if (!height) {
		height = getmaxy(stdscr) - 2;
	}

	/* init the simulation */
	World *world = new World(height, width, wrap, edit);
	if (!world) {
		goto fail;
	}

	while (world->life_cells()) {
		printw("Iteration #%" PRIuMAX " Life Cells: %" PRIuMAX "\n",
		       world->iteration(), world->life_cells());

		world->simulate_step();
		world->print(ch_cell_life, ch_cell_dead);

		char ch = getch();
		if (ch == 'q') {
			break;
		}

		world->do_state_transition();

		erase();
	}

	timeout(-1);
	printw("%" PRIuMAX " iterations have been simulated. ",
	       world->iteration());
	if (world->life_cells()) {
		printw("%" PRIuMAX " cells are still alive.\n",
		       world->life_cells());
	} else {
		printw("All life ceased to exist\n");
	}
	refresh();
	delete world;
	getch();

fail:
	return endwin();
}
