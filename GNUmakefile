LIBDIR ?= $(PWD)/lib

CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -Wpedantic -Wall -Wextra

all: insomnia lib/insomnia-strftime lib/insomnia-unix lib/insomnia-tail

insomnia: insomnia.in
	sed 's|@LIBDIR@|$(LIBDIR)|' < $< > $@
	chmod +x $@

lib/insomnia-strftime: src/insomnia-strftime.c
	$(CC) -o $@ $< $(CFLAGS) -D_POSIX_C_SOURCE=200809L $(LDFLAGS)

lib/insomnia-unix: src/insomnia-unix.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

lib/insomnia-tail: src/insomnia-tail.c
	$(CC) -o $@ $< $(CFLAGS) -D_POSIX_C_SOURCE=200809L -pthread $(LDFLAGS)
