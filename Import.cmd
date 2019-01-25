@echo off

rem Author   KMS - Martin Dubois, ing.
rem Product  OpenNet_Public
rem File     Import.cmd
rem Usage    Import.cmd

echo Executing  Import.cmd  ...

rem ===== Configuration =====================================================

set EXPORT_FOLDER=K:\Export

rem ===== Dependencies ======================================================

set OPEN_NET="%EXPORT_FOLDER%\OpenNet\0.0.7_KI_Windows"

rem ===== Constants =========================================================

set  DST_FOLDER="%CD%\Import"

rem ===== Verification ======================================================

if not exist %OPEN_NET% (
	echo FATAL ERROR  %OPEN_NET%  does not exist
	pause
	exit /B 1
)

rem  ===== Execution ========================================================

if not exist %DST_FOLDER% mkdir %DST_FOLDER%

pushd %OPEN_NET%
    call Import.cmd %DST_FOLDER%
popd

if ERRORLEVEL 1 (
	echo ERROR  call Import.cmd %DST_FOLDER%  failed - %ERRORLEVEL%
	pause
	exit /B 2
)

rem  ===== End ==============================================================

echo  OK
