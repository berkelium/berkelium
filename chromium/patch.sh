#!/bin/bash
FILES="src/chrome/browser/ui/gtk/browser_window_gtk.cc"
FILES="${FILES} src/chrome/app/chrome_main.cc"
FILES="${FILES} src/chrome/chrome.gyp"
FILES="${FILES} src/chrome/chrome_exe.gypi"
if [ "$1" = "apply" ]; then
	patch -p0 < chromium-berkelium.patch
elif [ "$1" = "revert" ]; then
	patch -R -p0 < chromium-berkelium.patch
elif [ "$1" = "diff" ]; then
	LANG=C svn diff $FILES
elif [ "$1" = "create" ]; then
	$0 diff > chromium-berkelium.patch
else
	echo "Syntax: $0 [apply|revert|diff|create]"
fi
