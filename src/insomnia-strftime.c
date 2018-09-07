#include <err.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>

static int prevday = -1;

#define NXFMT "%d %b %Y"
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
		if (!(sep = strchr(line, DELIM))) {
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

		if (prevday != -1 && prevday != tm->tm_yday) {
			if (strftime(tbuf, sizeof(tbuf), NXFMT, tm))
				printf("Day changed to %s\n", tbuf);
			else
				warnx("strftime failed for NXFMT");
		}

		prevday = tm->tm_yday;
		if (strftime(tbuf, sizeof(tbuf), TMFMT, tm))
			tptr = tbuf;
		else
			warnx("strftime failed for TMFMT");

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
