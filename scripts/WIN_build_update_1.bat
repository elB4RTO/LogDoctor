
@ECHO off

ECHO Initializing

:: Store the actual path
FOR /F "tokens=* USEBACKQ" %%e IN (`cd`) DO SET actual_path=%%e

:: Get the path of LogDoctor-git's folder and move in
SET logdocdir=%~dp0
SET logdocdir=%logdocdir:\=/%
IF "%logdocdir:~-1%"=="/" SET logdocdir=%logdocdir:~0,-1%
SET logdocdir=%logdocdir%/..
cd %logdocdir%


:: Check for a previous installation
IF NOT EXIST "C:\Program Files\LogDoctor" (
	ECHO Warning: no previous installation detected, please run the 'install' scripts instead
	cd "%actual_path%"
	PAUSE
	EXIT /B 0
)


:: Call the builder
IF NOT EXIST "scripts/builder.bat" (
	ECHO:
	ECHO Error: builder not found
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

CALL scripts/builder.bat "%actual_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Compilation failed
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


:: Start updating LogDoctor
ECHO "Starting update process"
cd ../installation_stuff/


SET data_path=%USERPROFILE%\AppData\Local\LogDoctor
IF EXIST "%data_path%" GOTO :step1

mkdir "%data_path%"
IF ERRORLEVEL 1 (
	ECHO Error: failed to create directory: '%data_path%'
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


:step1

xcopy /E /V /Y  logdocdata %data_path%\
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy LogDoctor's data
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


:: End Of Part 1
ECHO:
ECHO First part is done, please run 'WIN_build_update_2' as Administrator
cd "%actual_path%"
PAUSE
