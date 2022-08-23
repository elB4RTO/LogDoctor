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
if [ ! -e /usr/bin/logdoctor ]
	then
		echo "Warning: no previous installation detected, please run the installation script instead"
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
		mv LogDoctor logdoctor
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


if [ ! -e ~/.local/share/LogDoctor ]
	else
		mkdir -p ~/.local/share/LogDoctor
		if [[ "$?" != "0" ]]
			then
				echo "Error: failed to create directory: ~/.local/share/LogDoctor"
				exit
	fi
cp -r --no-preserve=all ./logdocdata/* ~/.local/share/LogDoctor/
if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy LogDoctor's data"
		exit
	fi


cp --no-preserve=all ./LogDoctor.desktop ~/.local/share/applications/
if [[ "$?" != "0" ]]
	then
		echo "Error: failed to create a menu entry"
		exit
	fi


sudo cp --no-preserve=all ./logdoctor.svg /usr/share/icons
if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy LogDoctor's icon"
		exit
	fi


cd ../build
cp --no-preserve=all ./logdoctor ./logdoctor.copy
chmod +x ./logdoctor.copy
sudo mv ./logdoctor.copy /usr/bin/logdoctor
if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy the executable"
		exit
	fi


# Update finished
echo "Update finished"
cd "$actual_path"
