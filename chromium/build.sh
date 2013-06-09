#!/bin/bash
if [ "$1" = "checkout" ]; then
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
elif [ "$1" = "update" ]; then
	depot_tools/gclient sync
elif [ "$1" = "debug" ]; then
	cd src
	./build/gyp_chromium -D component=shared_library
	make -j8 V=1 out/Debug/chrome
elif [ "$1" = "release" ]; then
	cd src
	make -j8 BUILDTYPE=Release V=1 out/Release/chrome
	cd -
else
	echo "Syntax: $0 [checkout|update|debug|release]"
fi
