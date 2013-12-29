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

goto debug

:release
set build=Release
set d= 
set r=x
goto start

:debug
set build=Debug
set d=x
set r= 
goto start

:start
echo.
echo.
echo Berkelium Windows Build Script
echo ==============================
echo.
echo s) setup ninja and msvc project files
echo.
echo d) (%d%) Debug Version
echo r) (%r%) Release Version
echo.
echo c) build chromium
echo b) build berkelium
echo.
echo p) Patch Chromium Source
echo q) Quit
if exist win_toolchain goto skip_download
echo.
echo t) Download Windows Toolchain (recommended!)
:skip_download
echo.
set /p action="Select: " %=%

if "%action%"=="s" (
	rem set GYP_DEFINES=component=shared_library
	set GYP_GENERATORS=ninja,msvs
	cd src
	call ..\depot_tools\python.bat build/gyp_chromium -D component=shared_library
	cd ..
) else if "%action%"=="d" (
	goto debug
) else if "%action%"=="r" (
	goto release
) else if "%action%"=="c" (
	cd src
	..\depot_tools\ninja -C out/%build% chrome
	cd ..
) else if "%action%"=="b" (
	cd src
	..\depot_tools\ninja -C out/%build% berkelium
	cd ..
) else if "%action%"=="p" (
	goto patch
) else if "%action%"=="q" (
	goto exit
) else if "%action%"=="t" (
	if not exist win_toolchain goto download_toolchain
	goto error
) else (
	goto error
)

goto start

:error
echo Error.. Please Select %ask%
goto start

:download_toolchain
rem http://www.chromium.org/developers/how-tos/build-instructions-windows#TOC-Automatic-simplified-toolchain-setup
call python src\tools\win\toolchain\toolchain.py

goto load_toolchain

:patch

echo.
echo.
echo Chromium Berkelium Patch
echo ========================
echo.
echo a) apply patch to chromium sources
echo r) remove (revert) patch from chromium sources
echo c) create new patch from chromium sources
echo s) show current patch
echo.
echo q) back to main menu
echo.

set /p action="Select (a, r, c, s, q): " %=%

if "%action%"=="c" (
	call goto patch_create
) else if "%action%"=="a" (
	echo TODO...
	call depot_tools\python.bat depot_tools\patch.py -p0 < chromium-berkelium.patch
) else if "%action%"=="r" (
	echo TODO...
	call depot_tools\python.bat depot_tools\patch.py -R -p0 < chromium-berkelium.patch
) else if "%action%"=="s" (
	goto patch_show
) else if "%action%"=="q" (
	goto start
) else (
	goto patch
)

goto patch

:patch_create

rem join all lines into one line
(for /f %%f in (patchfiles.txt) do echo | set /p=%%f ) > tmp.txt
rem read into %files%
set /p files=< tmp.txt
del tmp.txt
call svn diff %files% > tmp.txt

rem evil hack to normalise line endings...
del chromium-berkelium.patch
for /f "delims=" %%l in (tmp.txt) do (
	echo.%%l>> chromium-berkelium.patch
)
del tmp.txt

goto patch

:patch_show

for /f %%f in (patchfiles.txt) do (
	call svn diff %%f
) | more

goto patch

:end
pause

:exit
