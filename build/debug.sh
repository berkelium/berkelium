#!/bin/bash
clear

function filter()
{
	grep \
	-v \
	-e Chrome_FileThread \
	-e Chrome_IOThread \
	-e WorkerPool \
	-e BrowserBlockingWorker \
	-e Chrome_FileUserBlockingThread \
	-e Chrome_SafeBrowsingThread \
	-e Chrome_DBThread \
	-e Chrome_ProcessLauncherThread \
	-e Chrome_ChildIOThread \
	-e Chrome_HistoryThread \
	-e inotify_reader \
	-e "NSS SSL ThreadWorker" \
	-e IndexedDB \
	-e BrowserWatchdog \
	-e "Proxy resolver" \
	-e MediaStreamDeviceThread \
	-e Chrome_CacheThread \
	-e AudioThread \
	-e NetworkChangeNotifier \
	-e CrRendererMain \
	-e renderer_crash_uploader \
	-e HTMLParserThread \
	-e CrUtilityMain
}

if [ -z "$1" ]; then
	tests="*.*"
else
	tests="$1"
fi
cmake -Wdev -G "Eclipse CDT4 - Unix Makefiles" . -DCMAKE_ECLIPSE_EXECUTABLE=$builddir/eclipse/eclipse -DCMAKE_BUILD_TYPE=debug || exit 1
make berkelium-api all || exit 1
../chromium/build.sh debug-berkelium || exit 1
killall -9 berkelium
(run/berkelium-cpp-test --berkelium-host2=run/host-fake --gtest_filter=$tests 2>&1) #| filter

