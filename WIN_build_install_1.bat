
@ECHO off

ECHO Initializing

:: Store the actual path
FOR /F "tokens=* USEBACKQ" %%e IN (`cd`) DO SET actual_path=%%e

:: Get the path of LogDoctor-git's folder and move in
SET logdocdir=%~dp0
SET logdocdir=%logdocdir:\=/%
IF "%logdocdir:~-1%"=="/" SET logdocdir=%logdocdir:~0,-1%
cd %logdocdir%


:: Check for a previous installation
IF EXIST "C:\Program Files\LogDoctor" (
	ECHO Warning: a previous installation exists, please run the 'update' scripts instead
	cd "%actual_path%"
	PAUSE
	EXIT /B 0
)


:: Call the builder
IF NOT EXIST "builder.bat" (
	ECHO:
	ECHO Error: builder not found
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

CALL builder.bat "%actual_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Compilation failed
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


:: Start installing LogDoctor
ECHO:
ECHO Starting installation process

cd ..\installation_stuff\

SET data_path=%USERPROFILE%\AppData\Local\LogDoctor
IF NOT EXIST "%data_path%" mkdir "%data_path%"

IF NOT EXIST "%data_path%\logdoctor.conf" GOTO :step11

:loop9
ECHO:
ECHO Warning: a configuration file already exists
ECHO If you choose 'YES' the current file will be overwritten
ECHO If you choose 'NO' the current file will be kept
SET /P agree=Overwrite? [y/n] :

IF "%agree%"=="y" (
	GOTO :step10
)
IF "%agree%"=="Y" (
	GOTO :step10
)
IF "%agree%"=="n" (
	GOTO :step11
)
IF "%agree%"=="N" (
	GOTO :step11
)
ECHO NOT A VALID ANSWER
GOTO :loop9

:step10

copy /V /Y logdoctor.conf %data_path%\
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy configuration file
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step11

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
ECHO First part is done, please run 'WIN_build_install_2' as Administrator
cd "%actual_path%"
PAUSE
