
@ECHO off

:: Store the actual path
SET actual_path=%1

:: Get the path of LogDoctor-git's folder and move in
SET logdocdir=%~dp0
SET logdocdir=%logdocdir:\=/%
IF "%logdocdir:~-1%"=="/" SET logdocdir=%logdocdir:~0,-1%
cd %logdocdir%


:: Check the existence of a previous installation
IF NOT EXIST "C:\Program Files\LogDoctor\LogDoctor.exe" GOTO :step1

:loop0
ECHO:
ECHO Warning: a previous installation already exists
ECHO If you choose to continue, the actual content will be overwritten
SET /P agree=Continue? [y/n] :

IF "%agree%"=="y" (
	GOTO :step1
)
IF "%agree%"=="Y" (
	GOTO :step1
)
IF "%agree%"=="n" (
	cd "%actual_path%"
	EXIT /B 1
)
IF "%agree%"=="N" (
	cd "%actual_path%"
	EXIT /B 1
)
ECHO NOT A VALID ANSWER
GOTO :loop0

:step1

SET exec_path=C:\Program Files\LogDoctor
IF EXIST "%exec_path%" GOTO :step2

mkdir "%exec_path%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to create directory: '%exec_path%'
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step2


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


IF NOT EXIST "%exec_path%\licenses" GOTO :step3

rmdir /S /Q "%exec_path%\licenses"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to remove old licenses
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:step3

xcopy /E /I /V /Y logdocdata\licenses "%exec_path%\licenses"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to copy licenses
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)


:: Installation finished
ECHO:
ECHO Installation finished
cd "%actual_path%"
PAUSE
