# insomnia

A frontend for the [hii][hii github] IRC client.

## Status

This is currently under development and I didn't replace my irssi-based
setup with this yet, thus it doesn't have a single user and is likely
full of bugs.

## Dependencies

insomnia depends on the following non-standard utilities:

* [hii][hii github] >= 0.2.0
* [input][input github]
* [tmux][tmux hompepage] >= [2.9][tmux pr 1487]

## Usage

hii needs to be built by invoking the following command:

	$ make

Afterwards the generated `insomnia` script can be invoked as follows:

	$ ./insomnia irc.hackint.eu '#hii'

This will only work if an `hii` instance for `irc.hackint.eu` is already
running in the background and if the user already joined the `#hii`
channel.

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
[tmux pr 1487]: https://github.com/tmux/tmux/pull/1487
