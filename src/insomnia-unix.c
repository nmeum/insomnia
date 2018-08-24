#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

static struct sockaddr_un addr;

int
main(int argc, char **argv)
{
	int sfd;
	ssize_t ret;
	char *path, buf[BUFSIZ];

	if (argc <= 1) {
		fprintf(stderr, "USAGE: %s PATH\n", argv[0]);
		return EXIT_FAILURE;
	}

	path = argv[1];
	if (strlen(path) >= sizeof(addr.sun_path)) {
		fprintf(stderr, "The given path is too long\n");
		return EXIT_FAILURE;
	}

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		err(EXIT_FAILURE, "socket failed");

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if (connect(sfd, (struct sockaddr*)&addr,
			sizeof(struct sockaddr_un)) == -1)
		err(EXIT_FAILURE, "connect failed");

	while ((ret = read(sfd, buf, sizeof(buf))) > 0) {
		if (write(STDOUT_FILENO, buf, ret) == -1)
			err(EXIT_FAILURE, "write failed");
	}
	if (ret == -1)
		err(EXIT_FAILURE, "read filed");

	return EXIT_SUCCESS;
}
