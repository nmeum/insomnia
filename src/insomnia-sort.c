#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <sys/types.h>

static char **lines;
static size_t nlines;

enum { delay = 5 };
static int threshold;

static int
compar(const void *s1, const void *s2)
{
	unsigned long long u1, u2;

	u1 = strtoull(*(char**)s1, NULL, 10);
	u2 = strtoull(*(char**)s2, NULL, 10);

	if (u1 > u2) {
		return 1;
	} else if (u1 < u2) {
		return -1;
	} else {
		return 0;
	}
}

static void
sortprint(void)
{
	size_t i;

	qsort(lines, nlines - 1, sizeof(char*), compar);
	for (i = 0; i < nlines - 1; i++)
		printf("%s", lines[i]);
	fflush(stdout);
}

static void
sigalarm(int num)
{
	(void)num;
	threshold = 1;
	sortprint();
}

int
main(void)
{
	char *line;
	size_t llen;
	struct sigaction act;
	sigset_t blockset, oldset;

	act.sa_flags = SA_RESTART;
	act.sa_handler = sigalarm;
	if (sigemptyset(&act.sa_mask) == -1)
		err(EXIT_FAILURE, "sigemptyset failed");
	if (sigaction(SIGALRM, &act, NULL))
			err(EXIT_FAILURE, "sigaction failed");

	if (sigemptyset(&blockset) == -1)
		err(EXIT_FAILURE, "sigemptyset failed");
	sigaddset(&blockset, SIGALRM);

	nlines = 1;
	if (!(lines = malloc(1 * sizeof(char*))))
		err(EXIT_FAILURE, "malloc failed");

	line = NULL;
	llen = 0;

	alarm(delay);
	while (getline(&line, &llen, stdin) != -1) {
		if (threshold) {
			free(lines);
			lines = NULL;
			nlines = 1;

			printf("%s", line);
			fflush(stdout);
			continue;
		}

		if (sigprocmask(SIG_BLOCK, &blockset, &oldset))
			err(EXIT_FAILURE, "signal blocking failed");

		if (!(lines[nlines - 1] = strdup(line)))
			err(EXIT_FAILURE, "strdup failed");
		if (!(lines = realloc(lines, ++nlines * sizeof(char*))))
			err(EXIT_FAILURE, "realloc failed");

		if (sigprocmask(SIG_SETMASK, &oldset, NULL))
			err(EXIT_FAILURE, "signal unblocking failed");
	}

	if (ferror(stdin))
		err(EXIT_FAILURE, "ferror failed");
	sortprint();

	return EXIT_SUCCESS;
}
