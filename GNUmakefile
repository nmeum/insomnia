BINTGTS = insomnia insomnia-pager
DATTGTS = inputrc
LIBTGTS = insomnia-color insomnia-input insomnia-monitor insomnia-monitor-bell \
	  insomnia-output insomnia-sort insomnia-strftime insomnia-strip-bell \
	  insomnia-tail insomnia-topic insomnia-track-topic insomnia-unix

BINFILES = $(BINTGTS:%=bin/%)
DATFILES = $(DATTGTS:%=data/%)
LIBFILES = $(LIBTGTS:%=lib/%)

PREFIX  ?= /usr/local
LIBDIR  ?= $(PREFIX)/libexec/insomnia
BINDIR  ?= $(PREFIX)/bin
DOCDIR  ?= $(PREFIX)/share/doc/insomnia
DATADIR ?= $(PREFIX)/share/insomnia

CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -D_POSIX_C_SOURCE=200809L
CFLAGS += -Wpedantic -Wall -Wextra

all: $(BINFILES) $(DATFILES) $(LIBFILES)
bin/%: bin/%.in
	sed -e 's|@LIBDIR@|$(LIBDIR)|' \
		-e 's|@DATADIR@|$(DATADIR)|' < $< > $@
	chmod +x $@

lib/insomnia-tail: LDFLAGS += -pthread
lib/insomnia-%: lib/insomnia-%.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

install: $(TARGETS) README.md
	install -dm755 "$(DESTDIR)$(BINDIR)" "$(DESTDIR)$(LIBDIR)" "$(DESTDIR)$(DATADIR)"
	install -Dm755 $(BINFILES) "$(DESTDIR)$(BINDIR)"
	install -Dm755 $(LIBFILES) "$(DESTDIR)$(LIBDIR)"
	install -Dm644 $(DATFILES) "$(DESTDIR)$(DATADIR)"
	install -Dm644 README.md "$(DESTDIR)$(DOCDIR)/README.md"

clean:
	@git clean -fdX

.PHONY: all install clean
