BINFILES = bin/insomnia bin/insomnia-strftime
DATFILES = data/inputrc
LIBFILES = lib/insomnia-color lib/insomnia-input lib/insomnia-monitor \
	   lib/insomnia-monitor-bell lib/insomnia-output lib/insomnia-sort \
	   lib/insomnia-strip-bell lib/insomnia-tail lib/insomnia-topic \
	   lib/insomnia-track-topic lib/insomnia-unix

PREFIX  ?= /usr/local
LIBDIR  ?= $(PREFIX)/libexec/insomnia
BINDIR  ?= $(PREFIX)/bin
DOCDIR  ?= $(PREFIX)/share/doc/insomnia
DATADIR ?= $(PREFIX)/share/insomnia

CFLAGS ?= -Werror -Os
CFLAGS += -std=c99 -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE
CFLAGS += -Wpedantic -Wall -Wextra

all: $(BINFILES) $(DATFILES) $(LIBFILES)
bin/insomnia-%: bin/insomnia-%.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
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
