TARGETS = insomnia lib/insomnia-strftime \
	  lib/insomnia-unix lib/insomnia-tail \
	  lib/insomnia-sort

PREFIX  ?= /usr/local
LIBDIR  ?= $(PREFIX)/libexec/insomnia
BINDIR  ?= $(PREFIX)/bin
DOCDIR  ?= $(PREFIX)/share/doc/insomnia
DATADIR ?= $(PREFIX)/share/insomnia

CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -D_POSIX_C_SOURCE=200809L
CFLAGS += -Wpedantic -Wall -Wextra

all: $(TARGETS)
insomnia: insomnia.in
	sed -e 's|@LIBDIR@|$(LIBDIR)|' \
		-e 's|@DATADIR@|$(DATADIR)|' < $< > $@
	chmod +x $@

lib/insomnia-strftime: src/insomnia-strftime.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
lib/insomnia-unix: src/insomnia-unix.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
lib/insomnia-tail: src/insomnia-tail.c
	$(CC) -o $@ $< $(CFLAGS) -pthread $(LDFLAGS)
lib/insomnia-sort: src/insomnia-sort.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

install: $(TARGETS) README.md
	install -Dm755 insomnia "$(DESTDIR)$(BINDIR)/insomnia"
	install -dm755 "$(DESTDIR)$(LIBDIR)" "$(DESTDIR)$(DATADIR)"
	install -Dm755 $(wildcard ./lib/*) "$(DESTDIR)$(LIBDIR)"
	install -Dm644 $(wildcard ./data/*) "$(DESTDIR)$(DATADIR)"
	install -Dm644 README.md "$(DESTDIR)$(DOCDIR)/README.md"

clean:
	rm -f $(TARGETS)

.PHONY: all install clean
