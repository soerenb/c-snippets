/*
 * ll.c - Linked list functions
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
#include <stdlib.h>
#include <foo.h>

struct llel {
	int	     value;
	struct llel *next;
};

/**
 * list_append_element() - Append element to linked list
 * @list	Pointer to list start
 * @value	Value of the element
 */
static void list_append_element(struct llel *list, const int value)
{
	if (list != NULL) {
		struct llel *temp = malloc(sizeof(struct llel));
		temp->value = value;
		temp->next = NULL;

		while (list->next != NULL)
			list = list->next;
		list->next = temp;
	}
}

/**
 * list_add_element() - Add element to linked list
 * @list	Pointer to list start
 * @index	Index of the element to add
 * @value	Value of the element
 * Returns pointer to list start
 */
static struct llel *list_add_element(struct llel *list,
		const unsigned int index, const int value)
{
	unsigned int i = 0;
	struct llel *start = list;
	struct llel *prev;
	struct llel *temp = malloc(sizeof(struct llel));
	temp->value = value;

	if (list == NULL) {
		temp->next = NULL;
		return temp;
	}

	if (index == 0) {
		temp->next = list;
		return temp;
	}

	while ((list != NULL) && (i < index)) {
		prev = list;
		list = list->next;
		i++;
	}

	prev->next = temp;
	temp->next = list;

	return start;
}

/**
 * list_rm_element() - Remove element from linked list
 * @list	Pointer to list start
 * @index	Index of the element to remove
 * Returns pointer to list start
 */
static struct llel *list_rm_element(struct llel *list, const unsigned int index)
{
	unsigned int i = 0;
	struct llel *prev = NULL;
	struct llel *start = list;

	if (list == NULL)
		return NULL;

	if (index == 0) {
		prev = list->next;
		free(list);
		return prev;
	}

	while ((list->next != NULL) && (i < index)) {
		prev = list;
		list = list->next;
		i++;
	}
	if (i != index)
		return start;

	prev->next = list->next;
	free(list);

	return start;
}

/**
 * list_get_middle() - Get middle element of linked list
 * @list	Pointer to list start
 * Returns pointer to the middle list element
 */
static struct llel *list_get_middle(struct llel *list)
{
	const struct llel *fast = list;
	if (list == NULL)
		return NULL;

	while (fast != NULL) {
		fast = fast->next;
		if (fast == NULL)
			return list;
		fast = fast->next;
		list = list->next;
	}
	return list;
}

/**
 * list_reverse() - Reverse linked list
 * @list	Pointer to list start
 * Returns new list start pointer
 */
static struct llel *list_reverse(struct llel *list)
{
	struct llel *next = NULL;
	while (list != NULL) {
		struct llel *temp = list->next;
		list->next = next;
		next = list;
		list = temp;
	}
	return next;
}

/**
 * list_clear() - Free linked list
 * @list	Pointer to list start
 * Returns NULL
 */
static struct llel *list_clear(struct llel *list)
{
	while (list != NULL) {
		struct llel *tmp = list;
		list = list->next;
		free(tmp);
	}
	return NULL;
}

/**
 * list_commands() - Print command list
 */
static void list_commands(void)
{
	printf("h\tPrint help\n");
	printf("a\tAppend element to list\n");
	printf("d\tAdd element to list\n");
	printf("r\tRemove element from list\n");
	printf("v\tReverse list\n");
	printf("m\tPrint value of middle list element\n");
	printf("i\tInitialize list\n");
	printf("c\tClear list\n");
	printf("q\tQuit\n");
}

int main(int argc, char *argv[])
{
	char cmd;
	char *buf;
	unsigned int idx;
	unsigned int tmp;
	struct llel *element;
	struct llel *list_start = NULL;

	printf(">>>>>> Linked List Sample Program <<<<<<\n");
	while (1) {
		printf("\nCurrent list:\n");

		element = list_start;
		idx = 0;
		while (element != NULL) {
			printf("element[%u].value = %d\n", idx++,
					element->value);
			element = element->next;
		}
		printf("------------------------------------\n");
		printf("\n");

		printf("> ");
		buf = foo_gets();
		cmd = buf[0];
		free(buf);

		switch (cmd) {
		case 'a':
			printf("Append element\n");
			printf("Enter value: ");
			buf = foo_gets();
			list_append_element(list_start, strtol(buf, NULL, 0));
			free(buf);
			break;
		case 'd':
			printf("Add element\n");
			printf("Enter value: ");
			buf = foo_gets();
			tmp = strtol(buf, NULL, 0);
			free(buf);
			printf("Enter index: ");
			buf = foo_gets();
			list_start = list_add_element(list_start,
					strtol(buf, NULL, 0), tmp);
			free(buf);
			break;
		case 'r':
			printf("Remove element\n");
			printf("Enter index: ");
			buf = foo_gets();
			list_start = list_rm_element(list_start, strtol(buf,
						NULL, 0));
			free(buf);
			break;
		case 'v':
			printf("Reverse list\n");
			list_start = list_reverse(list_start);
			break;
		case 'm':
		{
			int val = list_get_middle(list_start)->value;

			printf("Middle element:\n");
			printf("Value = %d\n", val);
			break;
		}
		case 'i':
			printf("Init list\n");
			if (!list_start) {
				for (tmp = 0; tmp < 15; tmp++)
					list_start =
						list_add_element(list_start,
								tmp, tmp);
			}
			break;
		case 'c':
			printf("Clear list\n");
			list_start = list_clear(list_start);
			break;
		case 'q':
			list_clear(list_start);
			return 0;
		case 'h':
		default:
			list_commands();
			break;
		}
	}

	return 0;
}
