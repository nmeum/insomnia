#include <err.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>

static int prevday = -1;

#define NXFMT "%d %b %Y"
#define CUFMT "%H:%M:%S"

enum {
	DELIM = ' ',
	MAXTM = 256,
};

int
main(int argc, char **argv)
{
	unsigned long long epoch;
	char *line, *sep, *tptr, *cfmt, *nfmt;
	struct tm *tm;
	size_t len;
	ssize_t read;
	char tbuf[MAXTM];
	int opt;

	nfmt = cfmt = NULL;
	while ((opt = getopt(argc, argv, "n:c:")) != -1) {
		switch (opt) {
		case 'n':
			nfmt = optarg;
			break;
		case 'c':
			cfmt = optarg;
			break;
		}
	}

	if (!nfmt)
		nfmt = NXFMT;
	if (!cfmt)
		cfmt = CUFMT;

	len = 0;
	line = NULL;

	while ((read = getline(&line, &len, stdin)) != -1) {
		if (!(sep = strchr(line, DELIM))) {
			printf("%s", line);
			continue;
		}

		*sep = '\0';
		tptr = line;

		if (!(epoch = strtoull(line, NULL, 10)) ||
				!(tm = localtime((time_t*)&epoch)))
			goto cont;

		if (prevday != -1 && prevday != tm->tm_yday) {
			if (strftime(tbuf, sizeof(tbuf), nfmt, tm))
				printf("Day changed to %s\n", tbuf);
			else
				warnx("strftime failed for '%s'", nfmt);
		}

		if (*nfmt != '\0')
			prevday = tm->tm_yday;
		if (*cfmt != '\0') {
			if (strftime(tbuf, sizeof(tbuf), cfmt, tm))
				tptr = tbuf;
			else
				warnx("strftime failed for '%s'", cfmt);
		}

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
