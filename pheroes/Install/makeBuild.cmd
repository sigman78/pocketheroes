@echo off

if "%1"=="" goto usage

del out\*.* /Q
rmdir Out
mkdir Out

Set PREFIX=PocketHeroes
Set VERSTR=v%1

echo Creating Game .cab file(s)...
Cabwiz.exe %PREFIX%.inf /cpu "SA2K3_ENG" "SA2K3_RUS"
rem move %PREFIX%.SA2K2_ENG.CAB out\%PREFIX%.SA2K2_ENG.%VERSTR%.cab
move %PREFIX%.SA2K3_ENG.CAB out\%PREFIX%.SA2K3_ENG.%VERSTR%.cab
rem move %PREFIX%.SA2K2_RUS.CAB out\%PREFIX%.SA2K2_RUS.%VERSTR%.cab
move %PREFIX%.SA2K3_RUS.CAB out\%PREFIX%.SA2K3_RUS.%VERSTR%.cab
rem del %PREFIX%.SA2K2_ENG.DAT
del %PREFIX%.SA2K3_ENG.DAT
rem del %PREFIX%.SA2K2_RUS.DAT
del %PREFIX%.SA2K3_RUS.DAT

echo Creating Game archives...
cd out
rem pkzip25 -add -lev=9 %PREFIX%.SA2K2_ENG.%VERSTR%.zip %PREFIX%.SA2K2_ENG.%VERSTR%.cab
pkzip25 -add -lev=9 %PREFIX%.SA2K3_ENG.%VERSTR%.zip %PREFIX%.SA2K3_ENG.%VERSTR%.cab
rem pkzip25 -add -lev=9 %PREFIX%.SA2K2_RUS.%VERSTR%.zip %PREFIX%.SA2K2_RUS.%VERSTR%.cab
pkzip25 -add -lev=9 %PREFIX%.SA2K3_RUS.%VERSTR%.zip %PREFIX%.SA2K3_RUS.%VERSTR%.cab

cd ..
echo Make install files
Set NSIS=C:\Programming\NSIS\makensis.exe 
rem Set NSIS="C:\Program Files\NSIS\makensis.exe"
rem %NSIS% PocketHeroes_2k2_eng.nsi
%NSIS% PocketHeroes_2k3_eng.nsi
rem %NSIS% PocketHeroes_2k2_rus.nsi
%NSIS% PocketHeroes_2k3_rus.nsi


echo Done.
goto end

:usage
echo Invalid arguments!

:end

