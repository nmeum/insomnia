#include <err.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>

static int prevday = -1;

#define INSTR stdin
#define CUFMT "%X"
#define NXFMT "%d %b %Y"

enum {
	MAXTM = 256,
};

static void
inloop(char *cfmt, char *nfmt)
{
	unsigned long long epoch;
	char *line, *tend, *tptr;
	char tbuf[MAXTM];
	struct tm *tm;
	size_t len;

	len = 0;
	line = NULL;

	while (getline(&line, &len, INSTR) != -1) {
		errno = 0;
		epoch = strtoull(line, &tend, 10);
		if (errno || !(tm = localtime((time_t*)&epoch)))
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
		printf("%s%s", tptr, tend);
	}

	if (ferror(INSTR))
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
		cfmt = CUFMT;
	if (!nfmt)
		nfmt = NXFMT;

	inloop(cfmt, nfmt);
	return EXIT_SUCCESS;
}
