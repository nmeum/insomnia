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
static int threshold;

enum {
	LINESTEP = 16,
};

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

	qsort(lines, nlines, sizeof(char*), compar);
	for (i = 0; i < nlines; i++)
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

static void
bufferline(char *line)
{
	size_t newsiz;

	if (nlines && nlines % LINESTEP == 0) {
		newsiz = (nlines + LINESTEP) * sizeof(char*);
		if (!(lines = realloc(lines, newsiz)))
			err(EXIT_FAILURE, "realloc failed");
	}

	if (!(lines[nlines++] = strdup(line)))
		err(EXIT_FAILURE, "strdup failed");
}

static void
sethandler(void)
{
	struct sigaction act;

	act.sa_flags = SA_RESTART;
	act.sa_handler = sigalarm;
	if (sigemptyset(&act.sa_mask) == -1)
		err(EXIT_FAILURE, "sigemptyset failed");
	if (sigaction(SIGALRM, &act, NULL))
			err(EXIT_FAILURE, "sigaction failed");
}

int
main(int argc, char **argv)
{
	static char *line;
	static size_t llen;
	sigset_t blockset, oldset;

	if (argc <= 1) {
		fprintf(stderr, "USAGE: %s DELAY\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (!(lines = malloc(LINESTEP * sizeof(char*))))
		err(EXIT_FAILURE, "malloc failed");

	sethandler();
	if (sigemptyset(&blockset) == -1)
		err(EXIT_FAILURE, "sigemptyset failed");
	sigaddset(&blockset, SIGALRM);

	alarm(atoi(argv[1]));
	while (getline(&line, &llen, stdin) != -1) {
		if (threshold) {
			free(lines);
			lines = NULL;
			nlines = 0;

			printf("%s", line);
			fflush(stdout);
			continue;
		}

		if (sigprocmask(SIG_BLOCK, &blockset, &oldset))
			err(EXIT_FAILURE, "signal blocking failed");
		bufferline(line);
		if (sigprocmask(SIG_SETMASK, &oldset, NULL))
			err(EXIT_FAILURE, "signal unblocking failed");
	}

	if (ferror(stdin))
		err(EXIT_FAILURE, "ferror failed");
	sortprint();

	return EXIT_SUCCESS;
}
