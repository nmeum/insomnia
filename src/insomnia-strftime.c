#include <err.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>

/* TODO: How do we handle day changes?
 *
 * With the current format day changes are not recorded at all in the
 * log. Prefixing every line with the current day seems to be too
 * verbose and decreases readiblity. irssi solves this by printing a
 * single line when the date changed, maybe we should also do this.
 */
#define TMFMT "%H:%M:%S"

enum {
	DELIM = ' ',
	MAXTM = 256,
};

int
main(void)
{
	unsigned long long epoch;
	char *line, *sep, *tptr;
	struct tm *tm;
	size_t len;
	ssize_t read;
	char tbuf[MAXTM];

	len = 0;
	line = NULL;

	while ((read = getline(&line, &len, stdin)) != -1) {
		if (!(sep= strchr(line, DELIM))) {
			warnx("invalid input line '%s'", line);
			printf("%s", line);
			continue;
		}

		*sep = '\0';
		tptr = line;

		if (!(epoch = strtoull(line, NULL, 10)) ||
				!(tm = localtime((time_t*)&epoch))) {
			warn("couldn't parse epoch '%s'", line);
			goto cont;
		}

		if (strftime(tbuf, sizeof(tbuf), TMFMT, tm))
			tptr = tbuf;
		else
			warnx("strftime failed");

cont:
		/* getline(3) should guarantee that there always is at
		 * least a newline after DELIM thus `++sep` should
		 * always point to a valid memory location. */
		printf("%s %s", tptr, ++sep);
	}
	if (ferror(stdin))
		errx(EXIT_FAILURE, "ferror failed");

	return EXIT_SUCCESS;
}
