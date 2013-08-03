/*
 * json.c - Processing JSON data
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
#include <json/json.h>

int main(int argc, char *argv[])
{
	unsigned int i, num_locations;
	struct array_list *locations;
	struct json_object *obj;
	struct json_object *root = json_object_from_file("./input.json");

	if (!root) {
		printf("ERROR parsing input\n");
		return -1;
	}

	obj = json_object_object_get(root, "name");
	printf("%s\n", json_object_get_string(obj));

	obj = json_object_object_get(root, "location");
	num_locations = json_object_array_length(obj);
	printf("Number of locations: %u\n", num_locations);

	locations = json_object_get_array(obj);

	printf("\n");
	for (i = 0; i < num_locations; i++) {
		unsigned int j, num_questions;
		struct json_object *obj2;
		struct array_list *questions;

		obj = array_list_get_idx(locations, i);
		obj2 = json_object_object_get(obj, "name");
		printf("Current location: %s\n",
				json_object_get_string(obj2));

		obj = json_object_object_get(obj, "question");
		num_questions = json_object_array_length(obj);
		printf("Number of questions for location '%s': %u\n",
				json_object_get_string(obj2), num_questions);
		printf("\n");

		questions = json_object_get_array(obj);

		for (j = 0; j < num_questions; j++) {
			unsigned int k;
			struct array_list *answers;

			obj = array_list_get_idx(questions, j);
			obj2 = json_object_object_get(obj, "question");
			printf("Question: %s\n", json_object_get_string(obj2));

			obj2 = json_object_object_get(obj, "answer");
			answers = json_object_get_array(obj2);

			for (k = 0; k < 3; k++) {
				struct json_object *tmp =
					array_list_get_idx(answers, k);
				printf("Answer #%d: %s\n", k + 1,
						json_object_get_string(tmp));
			}

			obj2 = json_object_object_get(obj, "correct_answer");
			printf("Correct answer: %u\n",
					json_object_get_int(obj2));

			printf("\n");
		}
		printf("\n");
	}

	json_object_put(root);

	return 0;
}
