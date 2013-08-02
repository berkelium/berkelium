#!/bin/bash
clear
builddir=`dirname $(readlink -f $0)`

cd $builddir
$builddir/clean.sh
cmake -G "Eclipse CDT4 - Unix Makefiles" $(readlink -f $builddir) -DCMAKE_ECLIPSE_EXECUTABLE=$builddir/eclipse/eclipse -DCMAKE_BUILD_TYPE=Debug && make all test
