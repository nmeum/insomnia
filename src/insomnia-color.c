#include <err.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

#define REGEXPR "^([0-9]+) \\(([^\\)]+)\\) (.*)$"
#define REGSUBS 4 /* 3 subexpressions +1 for entire expression */

#define LEN(X) (sizeof(X) / sizeof(X[0]))
#define MATCHLEN(X) ((int)(X->rm_eo - X->rm_so))

enum {
	CECHO = 37, /* white */
	CHIGH = 31, /* red */

	FBOLD = 1,  /* bold font */
	FNORM = 10, /* normal font */
};

/* See https://en.wikipedia.org/wiki/ANSI_escape_code#3/4_bit */
static int colors[] = {
	31, 32, 33, 34, 35, 36,
	91, 92, 93, 94, 95, 96,
};

static short eflag; /* use custom format for own nick */
static short mflag; /* highlight mentions */

static int
gencolor(char *str, size_t len)
{
	size_t i;
	int code;

	for (code = 0, i = 0; i < len; i++)
		code += str[i];

	return colors[code % LEN(colors)];
}

static void
printline(char *line, regmatch_t *time, regmatch_t *nick, regmatch_t *text)
{
	int color, attribute;
	char *suffix, *timestr, *nickstr, *textstr;

	/* in bounds due to regex */
	suffix = &line[strlen(line) - 2];

	timestr = &line[time->rm_so];
	nickstr = &line[nick->rm_so];
	textstr = &line[text->rm_so];

	if (eflag && *suffix == '\006') { /* send by own client */
		color = CECHO;
		attribute = FBOLD;
	} else {
		color = gencolor(nickstr, MATCHLEN(nick));
		attribute = FNORM;
	}

	printf("%.*s (\033[%d;%dm%.*s\033[0m) ",
	       MATCHLEN(time), timestr,
	       color, attribute,
	       MATCHLEN(nick), nickstr);

	if (mflag && *suffix == '\007') /* text contains mention */
		printf("\033[%dm%.*s\033[0m%s", CHIGH,
		       (int)(suffix - textstr), textstr, suffix);
	else
		printf("%s", textstr);
}

int
main(int argc, char **argv)
{
	int opt;
	regex_t reg;
	regmatch_t matches[REGSUBS];
	static char *line;
	static size_t len;

#ifdef __OpenBSD__
	if (pledge("stdio", NULL) == -1)
		err(EXIT_FAILURE, "pledge failed");
#endif

	while ((opt = getopt(argc, argv, "em")) != -1) {
		switch (opt) {
		case 'e':
			eflag = 1;
			break;
		case 'm':
			mflag = 1;
			break;
		}
	}

	if (regcomp(&reg, REGEXPR, REG_EXTENDED))
		errx(EXIT_FAILURE, "regcomp failed");

	while (getline(&line, &len, stdin) != -1) {
		if (regexec(&reg, line, REGSUBS, matches, 0))
			printf("%s", line);
		else
			printline(line, &matches[1], &matches[2], &matches[3]);

		fflush(stdout);
	}
	if (ferror(stdin))
		errx(EXIT_FAILURE, "ferror failed");

	return EXIT_SUCCESS;
}
