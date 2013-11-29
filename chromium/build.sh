#!/bin/bash
if [ "$1" = "checkout" ]; then
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
elif [ "$1" = "update" ]; then
	depot_tools/gclient sync
elif [ "$1" = "setup" ]; then
	cd src
	./build/gyp_chromium -D component=shared_library
elif [ "$1" = "debug" ]; then
	cd src
	../depot_tools/ninja -C out/Debug chrome
elif [ "$1" = "release" ]; then
	cd src
	../depot_tools/ninja -C out/Release chrome
	cd -
else
	echo "Syntax: $0 [checkout|update|setup|debug|release]"
fi
