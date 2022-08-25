
:: Store the actual path
SET actual_path=%CD%

:: Get the path of LogDoctor-git's folder and move in
SET docdir=%~dp1
cd "%docdir%"

:: Check the existence of a previous executable file
IF EXIST C:\ProgramFiles\LogDoctor\ {
	ECHO "Warning: the path C:\ProgramFiles\LogDoctor\ already exists"
	SET /P agree="IF you choose to continue, the actual content will be overwritten\nContinue? [y/n] : "

	IF "%agree%"!="y" {
		IF "%agree%"!="Y" {
			EXIT /B
		}
	}
}

:: Start the compilation process
ECHO "Starting compilation process"

:: Make a build folder
mkdir build && cd build

:: Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
IF ERRORLEVEL 1 {
	:: an error occured during preparation
	ECHO "Error: failed to prepare cmake files"
	EXIT /B
}

:: Build the project
cmake --build ./ --target all
IF ERRORLEVEL 1 {
	:: an error occured during compilation
	ECHO "Error: failed to compile"
	EXIT /B
}

:: Compilation finished
ECHO "Compilation finished"

:: Start installing LogDoctor
ECHO "Starting installation process"
cd ../installation_stuff/

IF EXIST %USERPROFILE%\AppData\Local\LogDoctor {
	IF EXIST %USERPROFILE%\AppData\Local\LogDoctor\logdoctor.conf {
		:: A configuration file already exists
		ECHO "Warning: a configuration file already exists"
		SET /P agree="IF you choose 'YES' the current file will be overwritten\nIF you choose 'NO' the current file will be kept\nOverwrite? [y/n] : "

		IF "%agree%"!="y" {
			IF "%agree%"!="Y" {
				EXIT /B
			}
		}

		copy logdoctor.conf %USERPROFILE%\AppData\Local\LogDoctor\
		IF ERRORLEVEL 1 {
			:: an error occured during compilation
			ECHO "Error: failed to copy configuration file"
			EXIT /B
		}

		copy logdocdata\* %USERPROFILE%\AppData\Local\LogDoctor\
		IF ERRORLEVEL 1 {
			:: an error occured during compilation
			ECHO "Error: failed to copy LogDoctor's data"
			EXIT /B
		}
	}

} ELSE {
	mkdir %USERPROFILE%\AppData\Local\LogDoctor
	IF ERRORLEVEL 1 {
		ECHO "Error: failed to create directory: %USERPROFILE%\AppData\Local\LogDoctor"
		EXIT /B
	}
	copy logdoctor.conf %USERPROFILE%\AppData\Local\LogDoctor\logdoctor.conf
	IF ERRORLEVEL 1 {
		ECHO "Error: failed to copy configuration file"
		EXIT /B
	}
	copy logdocdata\* %USERPROFILE%\AppData\Local\LogDoctor\
	IF ERRORLEVEL 1 {
		ECHO "Error: failed to copy LogDoctor's data"
		EXIT /B
	}
}


IF NOT EXIST C:\ProgramFiles\LogDoctor\ {
	runas /user:administrator mkdir C:\ProgramFiles\LogDoctor
	IF ERRORLEVEL 1 {
		ECHO "Error: failed to create directory: C:\ProgramFiles\LogDoctor"
		EXIT /B
	}
}


runas /user:administrator copy LogDoctor.exe C:\ProgramFiles\LogDoctor\
IF ERRORLEVEL 1 {
	ECHO "Error: failed to copy the executable"
	EXIT /B
}


runas /user:administrator copy ./logdoctor.svg C:\ProgramFiles\LogDoctor\LogDoctor.svg
IF ERRORLEVEL 1 {
	ECHO "Error: failed to copy the icon"
	EXIT /B
}


:: Installation finished
ECHO "Installation finished"
cd "%actual_path%"

