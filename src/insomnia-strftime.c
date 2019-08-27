#include <err.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>

static int prevday = -1;

static char *
xstrftime(char *fmt, struct tm *tm)
{
	static char tbuf[256];

	if (!strftime(tbuf, sizeof(tbuf), fmt, tm)) {
		warnx("strftime failed for '%s'", fmt);
		return NULL;
	}

	return tbuf;
}

static void
inloop(char *cfmt, char *nfmt)
{
	static char *line;
	static size_t llen;
	char *tend, *tptr, *tbuf;
	unsigned long long epoch;
	struct tm *tm;

	while (getline(&line, &llen, stdin) != -1) {
		errno = 0;
		epoch = strtoull(line, &tend, 10);
		if (errno || !(tm = localtime((time_t*)&epoch)))
			goto cont;

		if (prevday != -1 && prevday != tm->tm_yday) {
			if ((tbuf = xstrftime(nfmt, tm)))
				printf("Day changed to %s\n", tbuf);
		}

		if (*nfmt != '\0')
			prevday = tm->tm_yday;
		if (*cfmt != '\0') {
			if ((tbuf = xstrftime(cfmt, tm)))
				tptr = tbuf;
		}

cont:
		printf("%s%s", tptr, tend);
	}

	if (ferror(stdin))
		errx(EXIT_FAILURE, "ferror failed");
	free(line);
}

int
main(int argc, char **argv)
{
	char *cfmt, *nfmt;
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

	if (!cfmt)
		cfmt = "%X";
	if (!nfmt)
		nfmt = "%d %b %Y";

	inloop(cfmt, nfmt);
	return EXIT_SUCCESS;
}
