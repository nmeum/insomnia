#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>

#define HDRFMT "\n==> %s <==\n\n"

static int firstrun = 1;
static pthread_t lastthr;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void
readlines(char *name, FILE *stream)
{
	size_t llen;
	pthread_t thr;
	char *fmt, *line;

	line = NULL;
	llen = 0;

	while (getline(&line, &llen, stream) != -1) {
		if (pthread_mutex_lock(&mtx))
			err(EXIT_FAILURE, "pthread_mutex_lock failed");

		fmt = HDRFMT;
		if (firstrun)
			fmt += 1; /* skip newline */

		thr = pthread_self();
		if (firstrun || !pthread_equal(thr, lastthr))
			printf(fmt, name);

		printf("%s", line);
		fflush(stdout);

		lastthr = thr;
		firstrun = 0;

		if (pthread_mutex_unlock(&mtx))
			err(EXIT_FAILURE, "pthread_mutex_unlock failed");
	}
}

static void*
tail(void *arg)
{
	pid_t pid;
	FILE *stream;
	int ret, p[2], wstatus;
	char *fp;

	fp = arg;
	if (pipe(p))
		err(EXIT_FAILURE, "pipe failed");

	switch ((pid = fork())) {
	case 0:
		close(STDOUT_FILENO);
		dup(p[1]);

		execlp("tail", "tail", "-f", fp, (char*)NULL);
		err(EXIT_FAILURE, "execlp failed");
	case -1:
		err(EXIT_FAILURE, "fork failed");
	default:
		if (!(stream = fdopen(p[0], "r")))
		       err(EXIT_FAILURE, "fdopen failed");
		readlines(fp, stream);

		if (waitpid(pid, &wstatus, 0) == -1)
			err(EXIT_FAILURE, "waitpid failed");
		else if ((ret = WIFEXITED(wstatus)))
			exit(ret);

		if (close(p[0]) || close(p[1]))
			err(EXIT_FAILURE, "close failed");
	}

	return NULL;
}

int
main(int argc, char **argv)
{
	int i, nthrs;
	pthread_t *thrs;

	if (argc <= 1) {
		fprintf(stderr, "USAGE: %s FILE...\n", argv[0]);
		return EXIT_FAILURE;
	}

	nthrs = argc - 1;
	if (!(thrs = malloc(sizeof(pthread_t*) * nthrs)))
		err(EXIT_FAILURE, "malloc failed");

	for (i = 0; i < nthrs; i++)
		if (pthread_create(&thrs[i], NULL, tail, argv[i + 1]))
			err(EXIT_FAILURE, "pthread_create failed");
	for (i = 0; i < nthrs; i++)
		if (pthread_join(thrs[i], NULL))
			err(EXIT_FAILURE, "pthread_join failed");

	return EXIT_SUCCESS;
}
