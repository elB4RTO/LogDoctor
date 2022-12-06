#!/bin/bash

# Store the actual path
actual_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $0)"
cd "$docdir"

# Check cmake availability
if [[ $(which cmake) =~ ^/ ]]
then
	$()
else
	echo "Error: Cmake not found in PATH, please add it or compilation won't socceed"
	exit
fi

# Check Qt availability
if [[ $(which qmake) =~ ^/ ]]
then
	$()
else
	echo "Error: Qt-clang not found in PATH, please add it or compilation won't socceed"
	exit
fi

# Check the existence of a previous executable file
if [ ! -e /Applications/LogDoctor.app ]
then
	echo "Warning: no previous installation detected, please run the installation script instead"
	exit
fi

# Start the compilation process
echo "Starting compilation process"

# Make a build folder
mkdir build && cd build

# Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
if [[ "$?" != "0" ]]
then
	# an error occured during preparation
	echo "Error: failed to prepare cmake files"
	exit
fi

# Build the project
cmake --build ./ --target all
if [[ "$?" == "0" ]]
then
	# compiled succesfully
	chmod 755 ./LogDoctor.app/Contents/MacOS/LogDoctor
else
	# an error occured during compilation
	echo "Error: failed to compile"
	exit
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
		exit
	fi
fi
if [ ! -e ~/"Lybrary/Application Support/LogDoctor/help" ]
then
	rm -r ~/"Lybrary/Application Support/LogDoctor/help"
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove old resources: ~/Lybrary/Application Support/LogDoctor/help"
		exit
	fi
fi
cp -r ./logdocdata/help ~/"Lybrary/Application Support/LogDoctor"
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy LogDoctor's data"
	exit
fi


cp ./osx_bundle/* ../build/LogDoctor.app/Contents/
sudo rm -r /Applications/LogDoctor.app
sudo mv ../build/LogDoctor.app /Applications/
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy the executable"
	exit
fi


# Update finished
echo "Update finished"
cd "$actual_path"
