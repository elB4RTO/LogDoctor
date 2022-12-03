#!/bin/bash

# Store the actual path
current_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(realpath $0))"
cd "$docdir"

# Check cmake availability
if [[ $(which cmake) =~ ^/ ]]
then
	$()
else
	echo "$(tput setaf 1)Error:$(tput sgr0) Cmake is not installed"
	exit
fi

# Check the existence of a previous executable file
if [ ! -e /usr/bin/logdoctor ]
then
	echo "$(tput setaf 11)Warning:$(tput sgr0) no previous installation found, please run the $(tput bold)install$(tput sgr0) script instead"
	exit
fi

# Start the compilation process
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Building$(tput sgr0)"

# Make a build folder
if [ -e build ]
then
	rm -r build
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove existing build directory"
		exit
	fi
fi
mkdir build
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to create build directory"
	exit
fi
cd build

# Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
if [[ "$?" != "0" ]]
then
	# an error occured during preparation
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to prepare build files"
	exit
fi

# Build the project
cmake --build ./ --target all
if [[ "$?" == "0" ]]
then
	# compiled succesfully
	mv LogDoctor logdoctor
else
	# an error occured during compilation
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to compile"
	exit
fi

# Compilation finished
wait
echo "$(tput setaf 10)-->$(tput sgr0) Built succesfully"

# Start updating LogDoctor
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Updating$(tput sgr0)"
cd ../installation_stuff/


if [ ! -d /usr/share/LogDoctor ]
then
	sudo mkdir -p /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to create directory: /usr/share/LogDoctor"
		exit
	fi
fi
chmod 644 ./logdocdata/help/*/*
chmod 444 ./logdocdata/licenses/*
sudo rsync -r --delete ./logdocdata /usr/share/LogDoctor
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to copy LogDoctor's data"
	exit
fi


chmod 644 ./LogDoctor.svg
sudo install -DC ./LogDoctor.svg -t /usr/share/LogDoctor
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to copy LogDoctor's icon"
	exit
fi


chmod 644 ./LogDoctor.desktop
sudo install -DC ./LogDoctor.desktop -t /usr/share/applications
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to create a menu entry"
	exit
fi


cd ../build
chmod 755 ./logdoctor
sudo install -DC ./logdoctor -t /usr/bin
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to copy the executable"
	exit
fi


# Update finished
echo "$(tput setaf 10)-->$(tput sgr0) $(tput bold)Updated succesfully$(tput sgr0)"
cd "$current_path"
