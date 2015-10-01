#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <foo.h>

static int my_atoi(const char *s)
{
	int ret = 0;
	bool neg = false;

	if (!s)
		return 0;

	if (*s == '-') {
		neg = true;
		s++;
	}

	while (*s != '\0' && *s != ' ' && *s >= '0' && *s <= '9')
		ret = ret * 10 + *s++ - '0';

	if (neg)
		ret *= -1;

	return ret;
}

int main(int argc, char *argv[])
{
	int num, ref;
	char *num_str;

	if (argc > 1) {
		num_str = argv[1];
	} else {
		size_t len;

		num_str = foo_gets();
		len = strlen(num_str);
		if (num_str[len - 1] == '\n')
			num_str[len - 1] = '\0';
	}

	num = my_atoi(num_str);
	ref = atoi(num_str);
	if (num != ref)
		fprintf(stderr, "ERROR: mismatch: in:%s, out:%d, ref:%d\n",
			num_str, num, ref);
	else
		printf("in:%s, out:%d, ref:%d\n", num_str, num, ref);

	return 0;
}
