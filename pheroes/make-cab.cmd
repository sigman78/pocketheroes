@echo off
@rem CAB Build script
echo # CAB Build script v1.0

rem Test if environment is set
if NOT DEFINED PH_ROOT (
	echo ! Paths are not configured, please use 'cscript setenv.js' before proceeding!
	echo # Done
	exit
)

rem Test options

set LANGUAGE=Russian
set LNG=%LANGUAGE:~0,3%
set VER=1.03
set PRJNAME=PalmHeroes

set SOURCE=_out
set OUTDIR=_install
set TOOLS=%PH_ROOT%\Tools\bin
set IDATA=%PH_ROOT%\Install

rem Retrieve current date
rem for /f %%i in ('%TOOLS%\strdate.exe %%y%%m%%d') do set BUILD_DATE=%%i
for /f %%i in ('%TOOLS%\strdate.exe %%Y.%%m.%%d') do set BUILD_DATE=%%i

rem Convert to full path
pushd %SOURCE%
set SOURCE=%CD%
popd

rem Convert to full path
pushd %OUTDIR%
set OUTDIR=%CD%
popd

rem Double slashes
rem for /f "delims=\\" %%i in ('@echo %OUTDIR%') do if RES EQU "" ( set RES=%%i ) else ( set RES=%RES%\\%%i )
rem for /f "delims=\\" %%i in ("%OUTDIR%") do echo %%i %%j %%k

rem Dump settings
echo - Build language is %LANGUAGE%
echo - Build date is %BUILD_DATE%
echo - Build root is '%PH_ROOT%'
echo - Source directory is '%SOURCE%'
echo - Destination directory is '%OUTDIR%'

rem Ensure destination path exists
if NOT EXIST "%OUTDIR%" (
	echo - Destination directory does not exists, creating...
	mkdir %OUTDIR%
)

rem Check Maps directory exists and writable
echo "tmp" >%SOURCE%\Maps\probe.tmp
if NOT EXIST %SOURCE%\Maps\probe.tmp (
	echo ! Can't find map file directory, ensure it does exists!
	echo # Done
	exit
)
del /q %SOURCE%\Maps\probe.tmp >NUL

rem Prepare Build version descriptor
if exist %SOURCE%\build.ver del /q %SOURCE%\build.ver >NUL
echo %PRJNAME% %VER% %LANGUAGE% %BUILD_DATE% >%SOURCE%\build.ver

rem 1. Enlist all map files (note the language selection)
echo - Preparing list of maps...
set _MAPLIST=%OUTDIR%\map_list.txt
del /q %_MAPLIST% 2>NUL
dir /B %SOURCE%\Maps\*_%LANGUAGE%.phm >%_MAPLIST%
rem check number of lines in listing
for /f %%i in ('%TOOLS%\awk.exe "{ lineno++ } END{ print lineno }" %OUTDIR%\map_list.txt') do set _LINENO=%%i
echo - Totaly %_LINENO% map(s) enlisted...

if "%_LINENO%" == "0" (
	echo ! Fatal error - no map files found!
	echo # Done
	exit
)

rem shitty control characters!
rem double the slash character to correctly pass arg to AWK, i.e. \ -> \\
echo %SOURCE% >%TEMP%\tempsed.txt
for /f %%i in ('%TOOLS%\sed.exe -e "s/\\/\\\\/g" %TEMP%\tempsed.txt') do set SOURCEDQ=%%i
echo %OUTDIR% >%TEMP%\tempsed.txt
for /f %%i in ('%TOOLS%\sed.exe -e "s/\\/\\\\/g" %TEMP%\tempsed.txt') do set OUTDIRDQ=%%i

rem Prepare inf manifest
echo - Build INF from template
set _OUTPREFIX=%OUTDIR%\%PRJNAME%.%LNG%
set _INFFILE=%_OUTPREFIX%.inf
%TOOLS%\awk.exe -f %IDATA%\ppc_cab.awk -v LNG=%LNG% -v SRC=%SOURCEDQ% -v VER=%VER% -v INC=%OUTDIRDQ% %IDATA%\ppc_cab.inf >%_INFFILE%

rem Convert inf file to unicode (required CabWiz workaround)
echo - Convert INF to unicode
rem cscript %TOOLS%\a2u.js //NoLogo %_INFFILE% %_INFFILE%
cscript %TOOLS%\a2u.js //NoLogo %_INFFILE% %_INFFILE%
rem Invoke CABWIZ
echo - Invoking CABWIZ utility
%IDATA%\cabwiz.exe %_INFFILE% /cpu ARM /err %TEMP%/cabwiz.errors /dest %OUTDIR% >NUL
rem Parse errors
if exist %TEMP%\cabwiz.errors (
	rem Dump unicode > ascii conversion
	type %TEMP%\cabwiz.errors >%TEMP%\cabwiz.ascii
	rem Dump error log file
	for /f "delims=" %%i in (%TEMP%\cabwiz.ascii) do echo ! Cabwiz %%i
	rem if %%i NEQ "" exit
)

set CABNAME=%_OUTPREFIX%.ARM.CAB

if NOT EXIST %CABNAME% (
	echo ! Fatal error - failed to create CAB file!
	echo # Done
	exit
)

echo - Zipping the CAB file
pkzip25 -add -lev=9 %_OUTPREFIX%.zip %CABNAME% >NUL

echo - Making NSIS Installer
rem %NSIS_PATH%\
set EXEPATH=%_OUTPREFIX%.EXE
d:\Programming\NSIS\makensis.exe /DCAB=%CABNAME% /DOUTFILE=%EXEPATH% /DPRODUCT_VER=%VER% /DPRODUCT_LANG=%LNG% %PH_ROOT%\Install\PalmHeroes.nsi >%_OUTPREFIX%.nslog

echo - Cleanup output folder...
del %OUTDIR%\*.txt >NUL
del %OUTDIR%\*.inf >NUL
del %OUTDIR%\*.nslog >NUL

echo # Done

