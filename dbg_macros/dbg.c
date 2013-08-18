/*
 * dbg.c - Macro examples for uniform debug output
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
#include <stdio.h>

#ifndef LOG_LEVEL
# define LOG_LEVEL	9
#endif

#define pr_err(fmt, ...)	fprintf(stderr, "ERROR:%s(%d):%s(): " fmt, \
					__FILE__, __LINE__, __func__, \
					##__VA_ARGS__)
#if LOG_LEVEL > 0
# define pr_warn(fmt, ...)	fprintf(stderr, "WARNING:%s(%d):%s(): " fmt, \
					__FILE__, __LINE__, __func__, \
					##__VA_ARGS__)
#endif

#if LOG_LEVEL > 1
# define pr_info(fmt, ...)	fprintf(stderr, "INFO: " fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL > 2
# define pr_dbg(fmt, ...)	fprintf(stderr, "DEBUG:%s(%d):%s(): " fmt, \
					__FILE__, __LINE__, __func__, \
					##__VA_ARGS__)
#endif

#if LOG_LEVEL > 3
# define pr_trace(fmt, ...)	fprintf(stderr, "TRACE:%s(%d):%s(): " fmt, \
					__FILE__, __LINE__, __func__, \
					##__VA_ARGS__)
#endif

#ifndef pr_warn
# define pr_warn(...)
#endif

#ifndef pr_info
# define pr_info(...)
#endif

#ifndef pr_dbg
# define pr_dbg(...)
#endif

#ifndef pr_trace
# define pr_trace(...)
#endif

int main(int argc, char *argv[])
{
	pr_trace("argc:%d, argv:%p\n", argc, argv);
	pr_err("something went wrong\n");
	pr_warn("don't worry\n");
	pr_info("fyi\n");
	pr_dbg("bugger\n");

	return 0;
}
