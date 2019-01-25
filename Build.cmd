@echo off

rem Author   KMS - Martin Dubois, ing.
rem Product  OpenNet_Public
rem File     Build.cmd

echo Executing  Build.cmd  ...

rem ===== Initialization ====================================================

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\MSBuild\15.0\Bin\MSBuild.exe"
set OPTIONS="OpenNet_Public.sln" /target:rebuild

rem ===== Verification ======================================================

if not exist %MSBUILD% (
	echo ERREUR FATAL  %MSBUILD%  does not exist
    echo Install Visual Studio 2017
	pause
	exit /B 1
)

rem ===== Execution =========================================================

%MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=x64
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=x64  failed - %ERRORLEVEL%
	pause
	exit /B 2
)

%MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=x64
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=x64  failed - %ERRORLEVEL%
	pause
	exit /B 3
)

rem ===== End ===============================================================

echo OK
