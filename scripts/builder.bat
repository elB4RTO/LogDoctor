
@ECHO off

:: Store the actual path
SET actual_path=%1

:: Get Qt base path
SET qt_base_path=C:/Qt
IF EXIST "%qt_base_path%" GOTO :step0

SET qt_base_path=C:/Program Files/Qt
IF EXIST "%qt_base_path%" GOTO :step0

SET qt_base_path=C:/Program Files (x86)/Qt
IF EXIST "%qt_base_path%" GOTO :step0

SET qt_base_path=%USERPROFILE%\Qt
IF EXIST "%qt_base_path%" GOTO :step0

SET qt_base_path=NONE

:step0

SET qt_path=%qt_base_path%

IF "%qt_base_path%" == "NONE" GOTO :step1

SET qt_base_path=%qt_base_path:\=/%

IF "%qt_base_path:~-1%"=="/" SET qt_base_path=%qt_base_path:~0,-1%


:: Get Qt version
SET qt_path=%qt_base_path%/5.15.2
IF EXIST "%qt_path%" GOTO :step1

SET qt_path=%qt_base_path%/6.0
IF EXIST "%qt_path%" GOTO :step1

SET qt_path=%qt_base_path%/6.2
IF EXIST "%qt_path%" GOTO :step1

SET qt_path=%qt_base_path%/6.4
IF EXIST "%qt_path%" GOTO :step1

SET qt_path=NONE


:step1

IF NOT "%qt_path%" == "NONE" GOTO :step2

:loop1
ECHO:
ECHO Qt not found, please insert the path of your Qt installation, including the version
ECHO Ex: C:/your/path/to/Qt/5.15.2
SET /P qt_path= :
IF NOT EXIST "%qt_path%" (
	ECHO Error: the given path doesn't exist
	GOTO :loop1
)


:step2

SET qt_path=%qt_path:\=/%

IF "%qt_path:~-1%"=="/" SET qt_path=%qt_path:~0,-1%


:: Get CMake path
SET cmake_path=%qt_base_path%/Tools/CMake_64/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :step4

where /Q cmake
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where cmake`) DO SET cmake_path=%%e
	IF EXIST "%cmake_path%" GOTO :step4
)

SET cmake_path=C:/Program Files/CMake/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :step4

SET cmake_path=C:/Program Files (x86)/CMake/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :step4

SET cmake_path=C:/CMake/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :step4

SET cmake_path=C:/msys64/usr/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :step4

SET cmake_path=C:/msys64/mingw64/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :step4

:loop3
ECHO:
ECHO CMake not found, please insert the path of your CMake installation, including the executable
ECHO Ex: C:/your/path/to/cmake.exe
SET /P cmake_path= :
IF EXIST "%cmake_path%" (
	IF NOT "%cmake_path:~-9%" == "cmake.exe" (
		ECHO Error: the given path doesn't point to a cmake executable
		GOTO :loop3
	)
) ELSE (
	ECHO Error: the given path doesn't exist
	GOTO :loop3
)


:step4

SET cmake_path=%cmake_path:\=/%

:: Get the C++ compiler path
SET cxx_compiler=%qt_base_path%/Tools/mingw1120_64/bin/g++.exe
IF EXIST "%cxx_compiler%" GOTO :step6

SET cxx_compiler=%qt_base_path%/Tools/mingw810_64/bin/g++.exe
IF EXIST "%cxx_compiler%" GOTO :step6

where /Q g++
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where g++`) DO SET cxx_compiler=%%e
	IF EXIST "%cxx_compiler%" GOTO :step6
)

SET cxx_compiler=C:/msys64/mingw64/bin/g++.exe
IF EXIST "%cxx_compiler%" GOTO :step6

:loop5
ECHO:
ECHO G++ not found, please insert the path of the C++ compiler
ECHO Ex: C:/your/path/to/g++.exe
SET /P cxx_compiler= :
IF EXIST "%cxx_compiler%" (
	IF NOT "%cxx_compiler:~-7%" == "g++.exe" (
		ECHO Error: the given path doesn't point to a g++ executable
		GOTO :loop5
	)
) ELSE (
	ECHO Error: the given path doesn't exist
	GOTO :loop5
)


:step6

SET cxx_compiler=%cxx_compiler:\=/%


SET prefix_path=%qt_path%/mingw112_64
IF EXIST "%prefix_path%" GOTO :step7

SET prefix_path=%qt_path%/mingw81_64
IF EXIST "%prefix_path%" GOTO :step7

ECHO:
ECHO Error: CMAKE_PREFIX_PATH not found
PAUSE
EXIT /B 1


:step7

IF "%prefix_path:~-1%"=="/" SET prefix_path=%prefix_path:~0,-1%


SET qt_dir=%prefix_path%/lib/cmake/Qt5
IF EXIST "%qt_dir%" GOTO :step8

ECHO:
ECHO Error: QT_DIR not found
PAUSE
EXIT /B 1


:step8

IF "%qt_dir:~-1%"=="/" SET qt_dir=%qt_dir:~0,-1%


:: Check if JOM is in PATH
where /Q jom
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where jom`) DO SET jom_path=%%e
	IF EXIST "%jom_path%" GOTO :step9_a
)

SET jom_path=%qt_base_path%/Tools/QtCreator/bin/jom
IF EXIST "%jom_path%" SET PATH=%PATH%;%jom_path%

where /Q jom
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where jom`) DO SET jom_path=%%e
	IF EXIST "%jom_path%" GOTO :step9_a
)

ECHO:
ECHO Error: JOM not found in PATH, please add it or compilation won't succeed
PAUSE
EXIT /B 1


:step9_a

:: Check if libwinpthread-1.dll is in PATH
where /Q libwinpthread-1.dll
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where libwinpthread-1.dll`) DO SET lib_path=%%e
	IF EXIST "%lib_path%" GOTO :step9_b2
)

SET lib_path=%qt_base_path%/Tools/mingw1120_64/bin
IF EXIST "%lib_path%" GOTO :step9_b1

SET lib_path=%qt_base_path%/Tools/mingw810_64/bin
IF EXIST "%lib_path%" GOTO :step9_b1

SET lib_path=C:/msys64/mingw64/bin
IF EXIST "%lib_path%" GOTO :step9_b1

:step9_b1

SET PATH=%PATH%;%lib_path%

where /Q libwinpthread-1.dll
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where libwinpthread-1.dll`) DO SET lib_path=%%e
	IF EXIST "%lib_path%" GOTO :step9_b2
)

ECHO:
ECHO Error: libwinpthread-1.dll not found in PATH, please add it or compilation won't succeed
PAUSE
EXIT /B 1

:step9_b2


:: Start the compilation process
ECHO:
ECHO Starting compilation process
ECHO:

:: Make a build folder
IF EXIST build rmdir /S /Q build

mkdir build
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to create directory: 'build'
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)
cd build

:: Prepare build files
"%cmake_path%" "%logdocdir%/logdoctor" "-DCMAKE_BUILD_TYPE:STRING=MinSizeRel" "-DCMAKE_GENERATOR:STRING=NMake Makefiles JOM" "-DCMAKE_MAKE_PROGRAM:STRING=jom" "-DCMAKE_CXX_COMPILER:FILEPATH=%cxx_compiler%" "-DCMAKE_PREFIX_PATH:PATH=%prefix_path%" "-DQT_DIR:PATH=%qt_dir%"
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to prepare cmake files
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:: Build the project
"%cmake_path%" --build ./ --target all -j%NUMBER_OF_PROCESSORS%
IF ERRORLEVEL 1 (
	ECHO:
	ECHO Error: failed to compile
	cd "%actual_path%"
	PAUSE
	EXIT /B 1
)

:: Deploy the static libraries
IF EXIST LogDoctor rmdir /S /Q LogDoctor
mkdir LogDoctor
move LogDoctor.exe LogDoctor\
SET prefix_path=%prefix_path:/=\%
"%prefix_path%\bin\windeployqt.exe" LogDoctor\
copy /V /Y "%prefix_path%\bin\libstdc++-6.dll" LogDoctor\
copy /V /Y "%prefix_path%\bin\libwinpthread-1.dll" LogDoctor\
copy /V /Y "%prefix_path%\bin\libgcc_s_seh-1.dll" LogDoctor\

:: Compilation finished
ECHO:
ECHO Compilation finished
