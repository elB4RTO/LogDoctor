
@ECHO off

:: Store the actual path
SET actual_path=%1

:: Get the path of LogDoctor-git's folder and move in
SET logdocdir=%~dp0
SET logdocdir=%logdocdir:\=/%
IF "%logdocdir:~-1%"=="/" SET logdocdir=%logdocdir:~0,-1%
SET logdocdir=%logdocdir%/..
cd %logdocdir%


SET exec_path=C:\Program Files\LogDoctor

IF NOT EXIST "%exec_path%" GOTO :step1

rmdir /S /Q "%exec_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to renew the executable directory
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step1

xcopy /E /I /V /Y build\LogDoctor "%exec_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy the executable
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


SET link_path=C:\ProgramData\Microsoft\Windows\Start Menu\Programs\LogDoctor

IF NOT EXIST "%link_path%" GOTO :step2

rmdir /S /Q "%link_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to remove old menu entry
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step2

mkdir "%link_path%"

SET link_path=%link_path%\LogDoctor.exe

IF NOT EXIST "%link_path%" GOTO :step3

del "%link_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to remove symlink
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step3

mklink "%link_path%" "%exec_path%\LogDoctor.exe"


copy /V /Y installation_stuff\LogDoctor.svg "%exec_path%\LogDoctor.svg"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy the icon
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


:: Update finished
ECHO:
ECHO "Update finished"
cd "%actual_path%"
PAUSE
