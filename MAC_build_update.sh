#!/bin/bash

# Store the actual path
actual_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(realpath $0))"
cd "$docdir"

# Check cmake availability
if [[ $(which cmake) =~ ^/ ]]
	then
		$()
	else
		echo "Error: Cmake is not installed"
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
		echo "Done compiling"
		chmod 755 ./LogDoctor
		mv LogDoctor ../installation_stuff/LogDoctor.app/bin/
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
cp -r --no-preserve=all ./logdocdata/help ~/"Lybrary/Application Support/LogDoctor"
if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy LogDoctor's data"
		exit
	fi


cd ../build
cp --no-preserve=all ./LogDoctor.app ./LogDoctor.app.copy
sudo mv ./LogDoctor.app.copy /Applications/LogDoctor.app
if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy the executable"
		exit
	fi


# Update finished
echo "Update finished"
cd "$actual_path"
