CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -Wpedantic -Wall -Wextra

all: lib/insomnia-okchan lib/insomnia-strftime

lib/insomnia-okchan: src/insomnia-okchan.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

lib/insomnia-strftime: src/insomnia-strftime.c
	$(CC) -o $@ $< $(CFLAGS) -D_XOPEN_SOURCE $(LDFLAGS)
