#!/bin/bash
FILES="$(cat patchfiles.txt)"
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
