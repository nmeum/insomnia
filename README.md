# insomnia

A frontend for the [hii][hii github] IRC client.

## Screenshot

![insomnia screenshot](https://gist.githubusercontent.com/nmeum/3886bef9c4b07f42bcc4d15da1c0177e/raw/8d01fbe80a7843fec58f098fcdca591fecdc97a9/insomnia.png)

## Status

insomnia is just a collection of small programs and scripts which are
connected using tmux to create a simple irssi-like text-based user
interface for hii. As you might expected it's pretty hacky but mostly
works.

## Features

* Support for readline-like key bindings and tab completions for nicks.
* Support for highlight "notification" through bell characters.
	* Use this in combination with tmux's monitor-bell feature.
* Support for tracking topic changes through tmux pane titles.
* Support for colorful output.

## Dependencies

insomnia depends on the following non-standard utilities:

* [hii][hii github] >= 1.0.0
* [input][input github] >= 0.7.0
* [tmux][tmux homepage] >= [3.3][tmux window-resized]
* awk with `fflush()` (not in [POSIX.1‐2008][posix issue 634])

Additionally, the following software is required for compilation:

* A C99 compiler
* [GNU make][GNU make] (sorry!)

## Installation

To build and install insomnia run the following commands:

	$ make
	$ make install

Consult the `GNUmakefile` for environment variables which can be used to
configure the installation. The variables `LIBDIR` and `DATADIR` are
special and need to be set for both targets `make` and `make install`.

### Development setup

In case you don't want to install insomnia globally but just want to
experiment with it compile insomnia using the following commands:

	$ export LIBDIR="$(pwd)/lib" DATADIR="$(pwd)/data"
	$ make

Afterwards run `./bin/insomnia`.

## Usage

After a successful installation `insomnia` can be invoked as follows:

	$ insomnia irc.hackint.org '#hii'

This will only work if an `hii` instance for `irc.hackint.org` is
already running in the background and if the user already joined the
`#hii` channel.

## License

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.

[hii github]: https://github.com/nmeum/hii
[input github]: https://github.com/nmeum/input
[tmux homepage]: https://tmux.github.io
[tmux window-resized]: https://github.com/tmux/tmux/issues/2995
[posix issue 634]: http://austingroupbugs.net/view.php?id=634
[GNU make]: https://www.gnu.org/software/make/
