TARGETS = insomnia lib/insomnia-strftime \
	  lib/insomnia-unix lib/insomnia-tail

PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/libexec/insomnia
BINDIR ?= $(PREFIX)/bin
DOCDIR ?= $(PREFIX)/share/doc/insomnia

CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -Wpedantic -Wall -Wextra

all: $(TARGETS)
insomnia: insomnia.in
	sed 's|@LIBDIR@|$(LIBDIR)|' < $< > $@
	chmod +x $@

lib/insomnia-strftime: src/insomnia-strftime.c
	$(CC) -o $@ $< $(CFLAGS) -D_POSIX_C_SOURCE=200809L $(LDFLAGS)
lib/insomnia-unix: src/insomnia-unix.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
lib/insomnia-tail: src/insomnia-tail.c
	$(CC) -o $@ $< $(CFLAGS) -D_POSIX_C_SOURCE=200809L -pthread $(LDFLAGS)

install: $(TARGETS) README.md
	install -Dm755 insomnia "$(DESTDIR)$(BINDIR)/insomnia"
	install -dm755 "$(DESTDIR)$(LIBDIR)"
	install -Dm755 $(wildcard ./lib/*) "$(DESTDIR)$(LIBDIR)"
	install -Dm644 README.md "$(DESTDIR)$(DOCDIR)/README.md"

clean:
	rm -f $(TARGETS)

.PHONY: all install clean
