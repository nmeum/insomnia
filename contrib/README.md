# insomnia contrib

This directory contains scripts which are not officially supported or
subject to any robustness or stability criteria. This document contains
a section for each script contained in this directory further explaining
how it is intended to be used.

## termux-client

This script allows using insomnia from Android phones running
[Termux][termux website]. It is intended to be installed as a
[Termux Widget][termux widget].

It requires the following Termux apps to be installed:

* [com.termux][fdroid com.termux]
* [com.termux.widget][fdroid com.termux.widget]
* [com.termux.api][fdroid com.termux.api]

Additionally, the following command must be run inside a Termux session
to install all required package dependencies:

	pkg install termux-api openssh mosh jq

Additionally, `termux-client` itself must be configured by editing it
and setting the `HOST` and `IDIR` variables to appropriate values.
Afterwards, the script can be installed by creating the directory
`$HOME/.shortcuts` in Termux, copying `termux-client` to it, and marking
it executable. If successful, `termux-client` can be added to your home
screen as 1x1 widget.

[termux website]: https://termux.com/
[termux widget]: https://wiki.termux.com/wiki/Termux:Widget
[fdroid com.termux]: https://f-droid.org/packages/com.termux/
[fdroid com.termux.widget]: https://f-droid.org/packages/com.termux.widget/
[fdroid com.termux.api]: https://f-droid.org/packages/com.termux.api/
