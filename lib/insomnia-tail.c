#include <err.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void
sigchld(int num)
{
	int status, wstatus;

	(void)num;

	status = EXIT_SUCCESS;
	while (waitpid((pid_t)-1, &wstatus, WNOHANG) > 0) {
		if (WIFEXITED(wstatus) && status == EXIT_SUCCESS)
			status = WEXITSTATUS(wstatus);
	}

	if (status != EXIT_SUCCESS)
		exit(status);
}

static void
sethandler(void)
{
	struct sigaction act;

	act.sa_flags = SA_RESTART;
	act.sa_handler = sigchld;
	if (sigemptyset(&act.sa_mask) == -1)
		err(EXIT_FAILURE, "sigemptyset failed");
	if (sigaction(SIGCHLD, &act, NULL))
		err(EXIT_FAILURE, "sigaction failed");
}

/* thread-safe version of basename(dirname(fp)) */
static ssize_t
servername(char **dest, char *fp)
{
	char *p, *last, *prev;

	last = prev = NULL;
	p = &fp[strlen(fp)];

	while (p-- != fp) {
		if (*p != '/')
			continue;

		if (!last) {
			last = p;
		} else if (!prev) {
			prev = p;
			break;
		}
	}

	if (!last || !prev)
		return -1;

	*dest = ++prev;
	return last - prev;
}

static void
readlines(char *fp, FILE *stream)
{
	size_t llen, nlen;
	char *nickstart, *line, *name;

	line = NULL;
	llen = 0;

	name = NULL;
	if ((nlen = servername(&name, fp)) <= 0)
		return;

	while (getline(&line, &llen, stream) != -1) {
		if (!(nickstart = strchr(line, '[')))
			continue;
		if ((errno = pthread_mutex_lock(&mtx)))
			err(EXIT_FAILURE, "pthread_mutex_lock failed");

		printf("%.*s[%.*s %s", (int)(nickstart - line), line,
			(int)nlen, name, nickstart + 1);

		fflush(stdout);
		if ((errno = pthread_mutex_unlock(&mtx)))
			err(EXIT_FAILURE, "pthread_mutex_unlock failed");
	}

	if (ferror(stream))
		errx(EXIT_FAILURE, "ferror failed");
	free(line);
}

static void*
tail(void *arg)
{
	FILE *stream;
	int p[2];
	char *fp;

	fp = arg;
	if (pipe(p))
		err(EXIT_FAILURE, "pipe failed");

	switch (fork()) {
	case 0:
		close(p[0]); /* close unused read-end */
		close(STDOUT_FILENO);
		dup(p[1]);
		close(p[1]);

		execlp("tail", "tail", "-f", fp, (char*)NULL);
		err(EXIT_FAILURE, "execlp failed");
	case -1:
		err(EXIT_FAILURE, "fork failed");
	default:
		close(p[1]); /* close unused write-end */
		if (!(stream = fdopen(p[0], "r")))
		       err(EXIT_FAILURE, "fdopen failed");

		readlines(fp, stream);
		if (fclose(stream) == EOF)
			err(EXIT_FAILURE, "fclose failed");
	}

	return NULL;
}

int
main(int argc, char **argv)
{
	int i, nthrs;
	pthread_t *thrs;

#ifdef __OpenBSD__
	if (pledge("stdio proc exec", NULL) == -1)
		err(EXIT_FAILURE, "pledge failed");
#endif

	if (argc <= 1) {
		fprintf(stderr, "USAGE: %s FILE...\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Setup SIGCHLD handler */
	sethandler();

	nthrs = argc - 1;
	if (!(thrs = malloc(sizeof(pthread_t*) * nthrs)))
		err(EXIT_FAILURE, "malloc failed");

	for (i = 0; i < nthrs; i++)
		if ((errno = pthread_create(&thrs[i], NULL, tail, argv[i + 1])))
			err(EXIT_FAILURE, "pthread_create failed");
	for (i = 0; i < nthrs; i++)
		if ((errno = pthread_join(thrs[i], NULL)))
			err(EXIT_FAILURE, "pthread_join failed");

	return EXIT_SUCCESS;
}
