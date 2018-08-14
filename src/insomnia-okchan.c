#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

// This is a C implementation of the IsValidChannel function from girc.
//
// https://github.com/lrstanley/girc/blob/102f17f86306c2152a8c6188f9bb8b0e7288de31/format.go#L157

int
main(int argc, char **argv) {
	int i;
	char *channel, c;
	size_t len;

	if (argc <= 1) {
		fprintf(stderr, "USAGE: %s CHANNEL\n", argv[0]);
		return EXIT_FAILURE;
	}
	channel = argv[1];

	len = strlen(channel);
	if (len <= 1 || len > 50)
		return EXIT_FAILURE;
	if (!strchr("!#&*~+", *channel))
		return EXIT_FAILURE;

	if (*channel == '!') {
		if (len < 7)
			return EXIT_FAILURE;

		for (i = 0; i < 6; i++) {
			if (!isdigit(channel[i]) && !isupper(channel[i]))
				return EXIT_FAILURE;
		}
	}

	while ((c = *channel++)) {
		if (strchr("\0\a\r\n ,:", c))
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
