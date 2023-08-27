#!/bin/bash

# Store the actual path
actual_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(dirname $0))"
cd "$docdir"

# Check the existence of a previous executable file
if [ ! -e /Applications/LogDoctor.app ]
then
	echo "Warning: no previous installation detected, please run the 'install' script instead"
	exit 0
fi

# Check cmake availability
which cmake &> /dev/null
if [[ "$?" != "0" ]]
then
	echo "Error: Cmake not found in PATH, please add it or compilation won't socceed"
	echo "Tip: PATH+=:/path/of/CMake/bin"
	exit 1
fi

# Check Qt availability
which qmake &> /dev/null
if [[ "$?" != "0" ]]
then
	echo "Error: Qt-clang not found in PATH, please add it or compilation won't socceed"
	echo "Tip: PATH+=:/path/of/Qt/bins"
	exit 1
fi

# Start the compilation process
echo "Starting compilation process"

# Make a build folder
if [ -d build ]
then
	rm -r build
fi
mkdir build && cd build

# Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
if [[ "$?" != "0" ]]
then
	# an error occured during preparation
	echo "Error: failed to prepare cmake files"
	exit 1
fi

# Build the project
cmake --build ./ --target all -j$(sysctl -n hw.ncpu)
if [[ "$?" == "0" ]]
then
	# compiled succesfully
	chmod 755 ./LogDoctor.app/Contents/MacOS/LogDoctor
else
	# an error occured during compilation
	echo "Error: failed to compile"
	exit 1
fi

# Compilation finished
wait
echo "Compilation finished"

# Start installing LogDoctor
echo "Starting update process"
cd ../installation_stuff/


if [ ! -e ~/"Lybrary/Application Support/LogDoctor" ]
then
	mkdir -p ~/"Lybrary/Application Support/LogDoctor"
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to create directory: '~/Lybrary/Application Support/LogDoctor'"
		exit 1
	fi
fi
if [ -e ~/"Lybrary/Application Support/LogDoctor/help" ]
then
	rm -r ~/"Lybrary/Application Support/LogDoctor/help"
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove old resources: ~/Lybrary/Application Support/LogDoctor/help"
		exit 1
	fi
fi
cp -r ./logdocdata/help ~/"Lybrary/Application Support/LogDoctor"
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy LogDoctor's data"
	exit 1
fi


cp -r ./osx_bundle/{bin,doc,Resources,info.plist} ../build/LogDoctor.app/Contents/
sudo rm -r /Applications/LogDoctor.app
if [[ "$?" != "0" ]]
then
	echo "Error: failed to remove the old app bundle"
	exit 1
fi
sudo mv ../build/LogDoctor.app /Applications/
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy the app bundle"
	exit 1
fi


# Update finished
echo "Update finished"
cd "$actual_path"
