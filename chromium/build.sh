#!/bin/bash

base=`dirname $0`
src="$base/src"
code=1

if [ "$1" = "checkout" ]; then
	cd $base
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
	code=$?
	cd -
elif [ "$1" = "update" ]; then
	cd $dbase
	depot_tools/gclient sync
	code=$?
	cd -
elif [ "$1" = "setup" ]; then
	cd $src
	./build/gyp_chromium -D component=shared_library
	code=$?
	cd -
elif [ "$1" = "debug-berkelium" ]; then
	cd $src
	../depot_tools/ninja -C out/Debug berkelium
	code=$?
        cd -
elif [ "$1" = "release-berkelium" ]; then
	cd $src
	../depot_tools/ninja -C out/Release berkelium
	code=$?
	cd -
elif [ "$1" = "debug-chrome" ]; then
        cd $src
        ../depot_tools/ninja -C out/Debug chrome
	code=$?
	cd -
elif [ "$1" = "release-chrome" ]; then
        cd $src
        ../depot_tools/ninja -C out/Release chrome
	code=$?
        cd -
else
	echo "Syntax: $0 [checkout|update|setup|debug|release]"
fi
exit $code
