#!/bin/bash

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(dirname $(realpath $0)))"
cd "$docdir"

# Make a build folder
if [ -e build ]
then
	rm -r build
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove existing build directory"
		exit 1
	fi
fi
mkdir build
if [[ "$?" != "0" ]]
then
	echo "Error: failed to create build directory"
	exit 1
fi
cd build

# Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
if [[ "$?" != "0" ]]
then
	# an error occured during preparation
	echo "Error: failed to prepare build files"
	exit 1
fi

# Build the project
cmake --build ./ --target all -j$(nproc)
if [[ "$?" == "0" ]]
then
	# compiled succesfully
	mv LogDoctor logdoctor
else
	# an error occured during compilation
	echo "Error: failed to compile"
	exit 1
fi

# Compilation finished
wait

# Start installing LogDoctor
cd ../installation_stuff/

if [ ! -d /usr/share/LogDoctor ]
then
	mkdir -p /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to create directory: /usr/share/LogDoctor"
		exit 1
	fi
fi
chmod 644 -R ./logdocdata/help/*
chmod 444 -R ./logdocdata/licenses/*
which rsync &> /dev/null
if [[ "$?" == "0" ]]
then
	rsync -r --delete ./logdocdata /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy LogDoctor's data"
		exit 1
	fi
else
	cp -r ./logdocdata/{help,licenses} /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to copy LogDoctor's data"
		exit 1
	fi
fi

chmod 644 ./LogDoctor.svg
install -DC ./LogDoctor.svg -t /usr/share/LogDoctor
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy LogDoctor's icon"
	exit 1
fi

cd ../build
chmod 755 ./logdoctor
install -DC ./logdoctor -t /usr/bin
if [[ "$?" != "0" ]]
then
	echo "Error: failed to copy the executable"
	exit 1
fi

# Installation finished
cd /
