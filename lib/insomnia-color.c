#include <stdio.h>
#include <err.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LEN(X) (sizeof(X) / sizeof(X[0]))

/* See https://en.wikipedia.org/wiki/ANSI_escape_code#3/4_bit */
static int colors[] = {
	31, 32, 33, 34, 35, 36,
	91, 92, 93, 94, 95, 96,
};

static int
color(char *str, size_t len)
{
	size_t i;
	int code;

	for (code = 0, i = 0; i < len; i++)
		code += str[i];

	return colors[code % LEN(colors)];
}

int
main(void)
{
	char *line, *start, *end;
	size_t len, nicklen;

	len = 0;
	line = NULL;

	while (getline(&line, &len, stdin) != -1) {
		fflush(stdout); /* flush output from previous iteration */

		if (!(start = strchr(line, '(')) || !(end = strchr(++start, ')')))
			goto cont;
		if ((nicklen = end - start) <= 0)
			goto cont;

		printf("%.*s\033[%dm%.*s\033[0m%s", (int)(start - line), line,
			color(start, nicklen), (int)nicklen, start, end);
		continue;
cont:
		printf("%s", line);
	}

	if (ferror(stdin))
		errx(EXIT_FAILURE, "ferror failed");
	return EXIT_SUCCESS;
}
