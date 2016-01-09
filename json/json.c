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
#include <json-c/json.h>

int main(int argc, char *argv[])
{
	json_bool ret;
	unsigned int i, num_locations;
	struct array_list *locations;
	struct json_object *obj;
	struct json_object *root = json_object_from_file("./input.json");

	if (!root) {
		printf("ERROR parsing input\n");
		return -1;
	}

	ret = json_object_object_get_ex(root, "name", &obj);
	if (!ret) {
		printf("ERROR: invalid key: 'name'");
		return 3;
	}
	printf("%s\n", json_object_get_string(obj));

	ret = json_object_object_get_ex(root, "location", &obj);
	if (!ret) {
		printf("ERROR: invalid key: 'location'");
		return 3;
	}
	num_locations = json_object_array_length(obj);
	printf("Number of locations: %u\n", num_locations);

	locations = json_object_get_array(obj);

	printf("\n");
	for (i = 0; i < num_locations; i++) {
		unsigned int j, num_questions;
		struct json_object *obj2;
		struct array_list *questions;

		obj = array_list_get_idx(locations, i);
		ret = json_object_object_get_ex(obj, "name", &obj2);
		if (!ret) {
			printf("ERROR: invalid key: 'name'");
			return 3;
		}
		printf("Current location: %s\n",
				json_object_get_string(obj2));

		ret = json_object_object_get_ex(obj, "question", &obj);
		if (!ret) {
			printf("ERROR: invalid key: 'question'");
			return 3;
		}
		num_questions = json_object_array_length(obj);
		printf("Number of questions for location '%s': %u\n",
				json_object_get_string(obj2), num_questions);
		printf("\n");

		questions = json_object_get_array(obj);

		for (j = 0; j < num_questions; j++) {
			unsigned int k;
			struct array_list *answers;

			obj = array_list_get_idx(questions, j);
			ret = json_object_object_get_ex(obj, "question", &obj2);
			if (!ret) {
				printf("ERROR: invalid key: 'question'");
				return 3;
			}
			printf("Question: %s\n", json_object_get_string(obj2));

			ret = json_object_object_get_ex(obj, "answer", &obj2);
			if (!ret) {
				printf("ERROR: invalid key: 'answer'");
				return 3;
			}
			answers = json_object_get_array(obj2);

			for (k = 0; k < 3; k++) {
				struct json_object *tmp =
					array_list_get_idx(answers, k);
				printf("Answer #%d: %s\n", k + 1,
						json_object_get_string(tmp));
			}

			ret = json_object_object_get_ex(obj, "correct_answer",
							&obj2);
			if (!ret) {
				printf("ERROR: invalid key: 'correct_answer'");
				return 3;
			}
			printf("Correct answer: %u\n",
					json_object_get_int(obj2));

			printf("\n");
		}
		printf("\n");
	}

	json_object_put(root);

	return 0;
}
