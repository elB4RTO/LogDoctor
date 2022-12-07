#!/bin/bash

# Store the actual path
actual_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $0)"
cd "$docdir"

# Check the existence of a previous executable file
if [ -e /Applications/LogDoctor.app ]
then
	echo "Warning: a previous installation exists, please run the 'update' script instead"
	exit 0
fi

# Check CMake availability
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
cmake --build ./ --target all
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
echo "Starting installation process"
cd ../installation_stuff/

if [ -e ~/Lybrary/Preferences/LogDoctor ]
then
	if [ -e ~/Lybrary/Preferences/LogDoctor/logdoctor.conf ]
	then
		# A configuration file already exists
		while true;
		do
			echo "Warning: a configuration file already exists"
			printf "If you choose 'YES' the current file will be overwritten\nIf you choose 'NO' the current file will be kept\nOverwrite? [y/n] : "
			read agree
			case "$agree"
			in
				"y" | "Y" | [yY][eE][sS])
					printf "\n"
					cp ./logdoctor.conf ~/Lybrary/Preferences/LogDoctor/
					if [[ "$?" != "0" ]]
					then
						# an error occured during compilation
						echo "Error: failed to copy configuration file"
						exit 1
					fi
					break
				;;
				"n" | "N" | [nN][oO])
					break
				;;
				*)
					echo "Invalid answer"
				;;
			esac
		done
	fi
else
	mkdir -p ~/Lybrary/Preferences/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to create directory: ~/Lybrary/Preferences/LogDoctor"
		exit 1
	fi
	cp ./logdoctor.conf ~/Lybrary/Preferences/LogDoctor/
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy configuration file"
		exit 1
	fi
fi


if [ ! -e ~/"Lybrary/Application Support/LogDoctor" ]
then
	mkdir -p ~/"Lybrary/Application Support/LogDoctor"
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to create directory: ~/Lybrary/Application Support/LogDoctor"
		exit 1
	fi
fi
if [ ! -e ~/"Lybrary/Application Support/LogDoctor/help" ]
then
	rm -r ~/"Lybrary/Application Support/LogDoctor/help"
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove old resources: ~/Lybrary/Application Support/LogDoctor/help"
		exit 1
	fi
fi
cp -r ./logdocdata/help ~/"Lybrary/Application Support/LogDoctor/"
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy LogDoctor's data"
	exit 1
fi


cp -r ./osx_bundle/* ../build/LogDoctor.app/Contents/
if [ -e /Applications/LogDoctor.app ]
then
	sudo rm -r /Applications/LogDoctor.app
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove old bundle: /Applications/LogDoctor.app"
		exit 1
	fi
fi
sudo mv ../build/LogDoctor.app /Applications/
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy the executable"
	exit 1
fi


# Installation finished
echo "Installation finished"
cd "$actual_path"
