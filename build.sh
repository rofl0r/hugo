#!/bin/bash

# The listed build instructions in the old INSTALL file were leaving me without a
# proper config.h.in,  so I've made this build script for next time I forget :-)
aclocal
autoconf
autoheader
automake --add-missing

# Ugly hack ;-) On ARM Linux, the configure script won't be able to tell what the
# system is, so let's figure out what the user is building on here.
ARCH=$(uname -m)
if [ "$ARCH" == "aarch64" ] || [ "$ARCH" == "arm64" ]; then
    ./configure --build=aarch64-unknown-linux-gnu
else
    ./configure
fi

# Check if user has root priveleges. If they don't, make will probably fail.
if [ "$EUID" -ne 0 ]; then
    echo "You need to run make as root or with sudo."
else
    # Let the user run make install themselves if they want to. Tell them after make.
    make
fi
