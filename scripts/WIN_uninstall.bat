
@ECHO off

:: Start uninstalling
ECHO Starting uninstallation process


SET exec_path=C:\Program Files\LogDoctor

IF NOT EXIST "%exec_path%" (
	ECHO Warning: executable directory not found
	GOTO :step1
)

rmdir /S /Q "%exec_path%"
IF ERRORLEVEL 1 (
	ECHO Error: failed to remove the executable directory: %exec_path%
	PAUSE
	EXIT /B 1
)

:step1


SET link_path=C:\ProgramData\Microsoft\Windows\Start Menu\Programs\LogDoctor

IF NOT EXIST "%link_path%" (
	ECHO Warning: menu entry not found
	GOTO :step2
)

rmdir /S /Q "%link_path%"
IF ERRORLEVEL 1 (
	ECHO Error: failed to remove the menu entry: %link_path%
	PAUSE
	EXIT /B 1
)

:step2


SET data_path=%USERPROFILE%\AppData\Local\LogDoctor

IF NOT EXIST "%data_path%" (
	ECHO Warning: LogDoctor's data folder not found
	GOTO :step3
)

rmdir /S /Q "%data_path%"
IF ERRORLEVEL 1 (
	ECHO Error: failed to remove LogDoctor's data folder: %data_path%
	PAUSE
	EXIT /B 1
)

:step3


:: Uninstallation finished
ECHO:
ECHO Uninstallation finished
PAUSE
