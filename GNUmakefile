LIBDIR ?= $(PWD)/lib

CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -Wpedantic -Wall -Wextra

all: insomnia lib/insomnia-okchan lib/insomnia-strftime lib/insomnia-unix

insomnia: insomnia.in
	sed 's|@LIBDIR@|$(LIBDIR)|' < $< > $@
	chmod +x $@

lib/insomnia-okchan: src/insomnia-okchan.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

lib/insomnia-strftime: src/insomnia-strftime.c
	$(CC) -o $@ $< $(CFLAGS) -D_XOPEN_SOURCE $(LDFLAGS)

lib/insomnia-unix: src/insomnia-unix.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
