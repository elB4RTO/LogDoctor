
@ECHO off

:: Store the actual path
SET actual_path=%1

:: Get the path of LogDoctor-git's folder and move in
SET logdocdir=%~dp0
SET logdocdir=%logdocdir:\=/%
IF "%logdocdir:~-1%"=="/" SET logdocdir=%logdocdir:~0,-1%
cd %logdocdir%


SET exec_path=C:\Program Files\LogDoctor
IF EXIST "%exec_path%" GOTO :step1

mkdir "%exec_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to create directory: '%exec_path%'
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step1


copy /B /V /Y build\LogDoctor.exe "%exec_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy the executable
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


copy /V /Y installation_stuff\logdoctor.svg "%exec_path%\LogDoctor.svg"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy the icon
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

IF NOT EXIST "%exec_path%\licenses" GOTO :step2

rmdir /S /Q "%exec_path%\licenses"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to remove old licenses
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step2

xcopy /E /I /V /Y logdocdata\licenses "%exec_path%\licenses"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy licenses
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)



:: Update finished
ECHO:
ECHO "Update finished"
cd "%actual_path%"
PAUSE
