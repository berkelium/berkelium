@echo off

if not exist depot_tools (
	echo Please Download depot_tools from:
	echo.
	echo https://src.chromium.org/svn/trunk/tools/depot_tools.zip
	echo.
	echo and extract is to directory %CD%
	echo.
	echo  *****************************************************************
	echo  * IMPORTANT                                                     *
	echo  * ---------                                                     *
	echo  * DO NOT use drag-n-drop or copy-n-paste extract from Explorer, *
	echo  * this will not extract the hidden ".git" folder                *
	echo  * which is necessary for depot_tools to autoupdate itself.      *
	echo  * You can use "Extract all..." from the context menu though.    *
	echo  *****************************************************************

	goto end
)

if not exist depot_tools/.git (
	echo Something with yours 'depot_tools' directory is not correct, please check!
	goto end
)

set PATH=%PATH%;%CD%\depot_tools

:download_src
if exist src goto load_toolchain

echo Please press any key to download chromium sources...
pause
call depot_tools\gclient sync

goto download_src

:load_toolchain
if exist win_toolchain\env.bat (
	call win_toolchain\env.bat
)

:start
echo.
echo.
echo Berkelium Windows Build Script
echo ==============================
echo.
echo 0) setup ninja and msvc project files
echo 1) build debug berkelium
echo 2) build release berklium
echo 3) build debug chromium
echo 4) build release chromium
set ask=Select (0-4
if exist win_toolchain goto skip_download
echo d) Download Windows Toolchain (recommended!)
set ask=%ask%, d
:skip_download
set ask=%ask%): 
echo.
set /p action="%ask%" %=%

if "%action%"=="0" (
	rem set GYP_DEFINES=component=shared_library
	set GYP_GENERATORS=ninja,msvs
	cd src
	..\depot_tools\python.bat build/gyp_chromium -D component=shared_library
	cd ..
	goto end
) else if "%action%"=="1" (
	cd src
	..\depot_tools\ninja -C out/Debug berkelium
	cd ..
	goto end
) else if "%action%"=="2" (
	cd src
	..\depot_tools\ninja -C out/Release berkelium
	cd ..
	goto end
) else if "%action%"=="3" (
	cd src
	..\depot_tools\ninja -C out/Debug chromium
	cd ..
	goto end
) else if "%action%"=="4" (
	cd src
	..\depot_tools\ninja -C out/Release chromium
	cd ..
	goto end
) else if "%action%"=="d" (
	if not exist win_toolchain goto download_toolchain
)

echo Error.. Please Select %ask%
goto start

:download_toolchain
rem http://www.chromium.org/developers/how-tos/build-instructions-windows#TOC-Automatic-simplified-toolchain-setup
call python src\tools\win\toolchain\toolchain.py

goto start

:end
pause
