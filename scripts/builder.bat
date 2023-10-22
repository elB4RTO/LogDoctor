
@ECHO off

:: Store the actual path
SET actual_path=%1

:: Get Qt base path
SET qt_base_path=C:/Qt
IF EXIST "%qt_base_path%" GOTO :STEP_set_qt_path

SET qt_base_path=C:/Program Files/Qt
IF EXIST "%qt_base_path%" GOTO :STEP_set_qt_path

SET qt_base_path=C:/Program Files (x86)/Qt
IF EXIST "%qt_base_path%" GOTO :STEP_set_qt_path

SET qt_base_path=%USERPROFILE%/Qt
IF EXIST "%qt_base_path%" GOTO :STEP_set_qt_path

SET qt_base_path=NONE

:STEP_set_qt_path

SET qt_path=%qt_base_path%

IF "%qt_base_path%" == "NONE" GOTO :STEP_check_qt_path

SET qt_base_path=%qt_base_path:\=/%

IF "%qt_base_path:~-1%"=="/" SET qt_base_path=%qt_base_path:~0,-1%


:: Get Qt version
SET qt_path=%qt_base_path%/6.2.4
IF EXIST "%qt_path%" GOTO :STEP_check_qt_path

SET qt_path=%qt_base_path%/6.3.2
IF EXIST "%qt_path%" GOTO :STEP_check_qt_path

SET qt_path=%qt_base_path%/6.4.3
IF EXIST "%qt_path%" GOTO :STEP_check_qt_path

SET qt_path=%qt_base_path%/6.5.3
IF EXIST "%qt_path%" GOTO :STEP_check_qt_path

SET qt_path=%qt_base_path%/6.6.0
IF EXIST "%qt_path%" GOTO :STEP_check_qt_path

SET qt_path=NONE


:STEP_check_qt_path

IF NOT "%qt_path%" == "NONE" GOTO :STEP_set_cmake_path

:LOOP_ask_qt_path
ECHO:
ECHO Qt not found, please insert the path of your Qt installation, including the version
ECHO Eg: C:/your/path/to/Qt/6.2.4
SET /P qt_path= :
IF NOT EXIST "%qt_path%" (
	ECHO Error: the given path doesn't exist
	GOTO :LOOP_ask_qt_path
)


:STEP_set_cmake_path

SET qt_path=%qt_path:\=/%

IF "%qt_path:~-1%"=="/" SET qt_path=%qt_path:~0,-1%


:: Get CMake path
SET cmake_path=%qt_base_path%/Tools/CMake_64/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path

where /Q cmake
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where cmake`) DO SET cmake_path=%%e
	IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path
)

SET cmake_path=C:/Program Files/CMake/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path

SET cmake_path=C:/Program Files (x86)/CMake/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path

SET cmake_path=C:/CMake/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path

SET cmake_path=C:/msys64/usr/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path

SET cmake_path=C:/msys64/mingw64/bin/cmake.exe
IF EXIST "%cmake_path%" GOTO :STEP_set_cmake_prefix_path

:LOOP_ask_cmake_path
ECHO:
ECHO CMake not found, please insert the path of your CMake installation, including the executable
ECHO Eg: C:/your/path/to/cmake.exe
SET /P cmake_path= :
IF EXIST "%cmake_path%" (
	IF NOT "%cmake_path:~-9%" == "cmake.exe" (
		ECHO Error: the given path doesn't point to a cmake executable
		GOTO :LOOP_ask_cmake_path
	)
) ELSE (
	ECHO Error: the given path doesn't exist
	GOTO :LOOP_ask_cmake_path
)


:STEP_set_cmake_prefix_path

SET cmake_path=%cmake_path:\=/%


SET prefix_path=%qt_path%/mingw112_64
IF EXIST "%prefix_path%" GOTO :STEP_set_cxx_compiler_gcc

SET prefix_path=%qt_path%/mingw81_64
IF EXIST "%prefix_path%" GOTO :STEP_set_cxx_compiler_gcc

SET prefix_path=%qt_path%/mingw_64
IF EXIST "%prefix_path%" GOTO :STEP_set_cxx_compiler_gcc

SET prefix_path=%qt_path%/msvc2019_64
IF EXIST "%prefix_path%" GOTO :STEP_set_cxx_compiler_msvc

ECHO:
ECHO Error: failed to determine CMAKE_PREFIX_PATH
PAUSE
EXIT /B 1


:STEP_set_cxx_compiler_gcc

IF "%prefix_path:~-1%"=="/" SET prefix_path=%prefix_path:~0,-1%

:: Get the g++ compiler path
SET cxx_compiler=%qt_base_path%/Tools/mingw1120_64/bin/g++.exe
IF EXIST "%cxx_compiler%" GOTO :STEP_set_qt_dir

SET cxx_compiler=%qt_base_path%/Tools/mingw810_64/bin/g++.exe
IF EXIST "%cxx_compiler%" GOTO :STEP_set_qt_dir

where /Q g++
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where g++`) DO SET cxx_compiler=%%e
	IF EXIST "%cxx_compiler%" GOTO :STEP_set_qt_dir
)

SET cxx_compiler=C:/msys64/mingw64/bin/g++.exe
IF EXIST "%cxx_compiler%" GOTO :STEP_set_qt_dir

:LOOP_ask_gcc_path
ECHO:
ECHO G++ not found, please insert the path of the C++ compiler
ECHO Eg: C:/your/path/to/g++.exe
SET /P cxx_compiler= :
IF EXIST "%cxx_compiler%" (
	IF NOT "%cxx_compiler:~-7%" == "g++.exe" (
		ECHO Error: the given path doesn't point to a g++ executable
		GOTO :LOOP_ask_gcc_path
	)
) ELSE (
	ECHO Error: the given path doesn't exist
	GOTO :LOOP_ask_gcc_path
)

GOTO :STEP_set_qt_dir


:STEP_set_cxx_compiler_msvc

IF "%prefix_path:~-1%"=="/" SET prefix_path=%prefix_path:~0,-1%

:: Get the Microsoft Visual Studio path
SET vs_base_path=C:/Program Files/Microsoft Visual Studio
IF EXIST "%vs_base_path%" GOTO :STEP_set_vs_version

SET vs_base_path=C:/Program Files (x86)/Microsoft Visual Studio
IF EXIST "%vs_base_path%" GOTO :STEP_set_vs_version

:LOOP_ask_vs_path
ECHO:
ECHO Miscrosoft Visual Studio not found, please insert the path of you installation
ECHO Eg: C:/Program Files/Microsoft Visual Studio
SET /P vs_base_path= :
IF NOT EXIST "%vs_base_path%" (
	ECHO Error: the given path doesn't exist
	GOTO :LOOP_ask_vs_path
)

:STEP_set_vs_version

SET vs_base_path=%vs_base_path:\=/%

IF "%vs_base_path:~-1%"=="/" SET vs_base_path=%vs_base_path:~0,-1%


SET vs_path=%vs_base_path%/2022/Community
IF EXIST "%vs_path%" GOTO :STEP_set_cl_path

SET vs_path=%vs_base_path%/2019/Community
IF EXIST "%vs_path%" GOTO :STEP_set_cl_path

:LOOP_ask_vs_version_path
ECHO:
ECHO Visual Studio version not found, please insert the path of you version
ECHO Eg: C:/Program Files/Microsoft Visual Studio/2022/Community
SET /P vs_path= :
IF NOT EXIST "%vs_path%" (
	ECHO Error: the given path doesn't exist
	GOTO :LOOP_ask_msvc_path
)

SET vs_path=%vs_path:\=/%

IF "%vs_path:~-1%"=="/" SET vs_path=%vs_path:~0,-1%"

:STEP_set_cl_path

:: Get the cl compiler path
where /Q cl
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where cl`) DO SET cxx_compiler=%%e
	IF EXIST "%cxx_compiler%" GOTO :STEP_set_qt_dir
)

:STEP_get_cl_path_1

IF NOT EXIST "%vs_path%/VC/Tools/MSVC/14.35.32215" GOTO :STEP_get_cl_path_2

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.35.32215/bin/Hostx86/x64/cl.exe
IF EXIST "%cxx_compiler%" SET arch=x86_amd64 && GOTO :STEP_set_qt_dir

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.35.32215/bin/Hostx64/x64/cl.exe
IF EXIST "%cxx_compiler%" SET arch=amd64 && GOTO :STEP_set_qt_dir

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.35.32215/bin/Hostx86/x86/cl.exe
IF EXIST "%cxx_compiler%" SET arch=x86 && GOTO :STEP_call_vc_dev_vars

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.35.32215/bin/Hostx64/x86/cl.exe
IF EXIST "%cxx_compiler%" SET arch=amd64_x86 && GOTO :STEP_set_qt_dir

:STEP_get_cl_path_2

IF NOT EXIST "%vs_path%/VC/Tools/MSVC/14.29.30133" GOTO :LOOP_ask_cl_path

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.29.30133/bin/HostX86/x64/cl.exe
IF EXIST "%cxx_compiler%" SET arch=x86_amd64 && GOTO :STEP_set_qt_dir

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.29.30133/bin/HostX64/x64/cl.exe
IF EXIST "%cxx_compiler%" SET arch=amd64 && GOTO :STEP_set_qt_dir

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.29.30133/bin/HostX86/x86/cl.exe
IF EXIST "%cxx_compiler%" SET arch=x86 && GOTO :STEP_set_qt_dir

SET cxx_compiler=%vs_path%/VC/Tools/MSVC/14.29.30133/bin/HostX64/x86/cl.exe
IF EXIST "%cxx_compiler%" SET arch=amd64_x86 && GOTO :STEP_set_qt_dir

:LOOP_ask_cl_path
ECHO:
ECHO CL not found, please insert the path of the C++ compiler
ECHO Eg: C:/your/path/to/cl.exe
SET /P cxx_compiler= :
IF EXIST "%cxx_compiler%" (
	IF NOT "%cxx_compiler:~-6%" == "cl.exe" (
		ECHO Error: the given path doesn't point to a cl executable
		GOTO :LOOP_ask_cl_path
	)
) ELSE (
	ECHO Error: the given path doesn't exist
	GOTO :LOOP_ask_cl_path
)

:LOOP_ask_arch
ECHO:
ECHO Please select the architecture to use
ECHO Options: x86, x86_amd64, amd64, amd64_x86
SET /P arch= :
IF "%arch%"=="x86" GOTO :STEP_call_vc_dev_vars
IF "%arch%"=="x86_amd64" GOTO :STEP_call_vc_dev_vars
IF "%arch%"=="amd64" GOTO :STEP_call_vc_dev_vars
IF "%arch%"=="amd64_x86" GOTO :STEP_call_vc_dev_vars
ECHO Error: not a valid architecture
GOTO :LOOP_ask_arch

:STEP_call_vc_dev_vars

:: Call developer batch files to setup needed vars
CALL "%vs_path%\Common7\Tools\VsDevCmd.bat"
CALL "%vs_path%\VC\Auxiliary\Build\vcvarsall.bat %arch%"


:STEP_set_qt_dir

SET cxx_compiler=%cxx_compiler:\=/%


SET qt_dir=%prefix_path%/lib/cmake/Qt6
IF EXIST "%qt_dir%" GOTO :STEP_check_path

ECHO:
ECHO Error: failed to determine QT_DIR, Qt6 not found
PAUSE
EXIT /B 1


:STEP_check_path

IF "%qt_dir:~-1%"=="/" SET qt_dir=%qt_dir:~0,-1%


:: Check if Qt stuff is in PATH
where /Q qtcreator
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where qtcreator`) DO SET qtcreator_path=%%e
	IF EXIST "%qtcreator_path%" GOTO :STEP_check_jom
)

SET qtcreator_path=%qt_base_path%/Tools/QtCreator/bin
IF EXIST "%qtcreator_path%" SET PATH=%PATH%;%qtcreator_path%


:STEP_check_jom


:: Check if JOM is in PATH
where /Q jom
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where jom`) DO SET jom_path=%%e
	IF EXIST "%jom_path%" GOTO :STEP_set_lib_path
)

SET jom_path=%qt_base_path%/Tools/QtCreator/bin/jom
IF EXIST "%jom_path%" SET PATH=%PATH%;%jom_path%

where /Q jom
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where jom`) DO SET jom_path=%%e
	IF EXIST "%jom_path%" GOTO :STEP_set_lib_path
)

ECHO:
ECHO Error: JOM not found in PATH, please add it or compilation won't succeed
PAUSE
EXIT /B 1


:STEP_set_lib_path

:: Check if libwinpthread-1.dll is in PATH
where /Q libwinpthread-1.dll
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where libwinpthread-1.dll`) DO SET lib_path=%%e
	IF EXIST "%lib_path%" GOTO :STEP_start_compiling
)

SET lib_path=%qt_base_path%/Tools/mingw1120_64/bin
IF EXIST "%lib_path%" GOTO :STEP_check_libwinpthread

SET lib_path=%qt_base_path%/Tools/mingw810_64/bin
IF EXIST "%lib_path%" GOTO :STEP_check_libwinpthread

SET lib_path=C:/msys64/mingw64/bin
IF EXIST "%lib_path%" GOTO :STEP_check_libwinpthread

SET lib_path=C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer/Git/mingw64/bin
IF EXIST "%lib_path%" GOTO :STEP_check_libwinpthread


:STEP_check_libwinpthread

SET PATH=%PATH%;%lib_path%

where /Q libwinpthread-1.dll
IF ERRORLEVEL 0 (
	FOR /F "tokens=* USEBACKQ" %%e IN (`where libwinpthread-1.dll`) DO SET lib_path=%%e
	IF EXIST "%lib_path%" GOTO :STEP_start_compiling
)

ECHO:
ECHO Error: libwinpthread-1.dll not found in PATH, please add it or compilation won't succeed
PAUSE
EXIT /B 1

:STEP_start_compiling


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

:: Compilation finished
ECHO:
ECHO Compilation finished
