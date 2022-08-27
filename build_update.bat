
:: Store the actual path
SET actual_path=%CD%

:: Get the path of LogDoctor-git's folder and move in
SET docdir=%~dp0
cd "%docdir%"

:: Check the existence of a previous executable file
IF NOT EXIST C:\ProgramFiles\LogDoctor\ (
	ECHO "Warning: no previous installation detected, please run the installation script instead"
	cd "%actual_path%"
	EXIT /B
)

:: Start the compilation process
ECHO "Starting compilation process"

:: Make a build folder
mkdir build && cd build

:: Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
IF ERRORLEVEL 1 (
	:: an error occured during preparation
	ECHO "Error: failed to prepare cmake files"
	cd "%actual_path%"
	EXIT /B
)

:: Build the project
cmake --build ./ --target all
IF ERRORLEVEL 1 (
	:: an error occured during compilation
	ECHO "Error: failed to compile"
	cd "%actual_path%"
	EXIT /B
)

:: Compilation finished
ECHO "Compilation finished"

:: Start installing LogDoctor
ECHO "Starting update process"
cd ../installation_stuff/


IF NOT EXIST %USERPROFILE%\AppData\Local\LogDoctor (
	mkdir %USERPROFILE%\AppData\Local\LogDoctor
	IF ERRORLEVEL 1 (
		ECHO "Error: failed to create directory: %USERPROFILE%\AppData\Local\LogDoctor"
		cd "%actual_path%"
		EXIT /B
	)
)
copy logdocdata\* %USERPROFILE%\AppData\Local\LogDoctor\
IF ERRORLEVEL 1 (
	ECHO "Error: failed to copy LogDoctor's data"
	cd "%actual_path%"
	EXIT /B
)


runas /user:administrator copy LogDoctor.exe C:\ProgramFiles\LogDoctor\
IF ERRORLEVEL 1 (
	ECHO "Error: failed to copy the executable"
	cd "%actual_path%"
	EXIT /B
)

runas /user:administrator copy ./logdoctor.svg C:\ProgramFiles\LogDoctor\LogDoctor.svg
IF ERRORLEVEL 1 (
	ECHO "Error: failed to copy the icon"
	cd "%actual_path%"
	EXIT /B
)


:: Update finished
ECHO "Update finished"
cd "%actual_path%"
