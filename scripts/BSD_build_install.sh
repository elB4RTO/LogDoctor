#!/usr/local/bin/bash

# Store the actual path
current_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(dirname $(realpath $0)))"
cd "$docdir"

# Check the existence of a previous executable file
if [ -e /usr/bin/logdoctor ]
then
	echo -e "\033[93mWarning:\033[0m a previous installation exists, please run the \033[1mupdate\033[0m script instead"
	exit 0
fi

# Check cmake availability
which cmake &> /dev/null
if [[ "$?" != "0" ]]
then
	echo -e "\033[31mError:\033[0m Cmake is not installed"
	exit 1
fi

# Start the compilation process
echo -e "\033[94m==>\033[0m \033[1mBuilding\033[0m"

# Make a build folder
if [ -e build ]
then
	rm -r build
	if [[ "$?" != "0" ]]
	then
		echo -e "\033[31mError:\033[0m failed to remove existing build directory"
		exit 1
	fi
fi
mkdir build
if [[ "$?" != "0" ]]
then
	echo -e "\033[31mError:\033[0m failed to create build directory"
	exit 1
fi
cd build

# Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=Release
if [[ "$?" != "0" ]]
then
	# an error occured during preparation
	echo -e "\033[31mError:\033[0m failed to prepare build files"
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
	echo -e "\033[31mError:\033[0m failed to compile"
	exit 1
fi

# Compilation finished
wait
echo -e "\033[92m-->\033[0m Built succesfully"

# Start installing LogDoctor
echo -e "\033[94m==>\033[0m \033[1mInstalling\033[0m"
cd ../installation_stuff/

if [ ! -d ~/.config/LogDoctor ]
then
	mkdir -p ~/.config/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo -e "\033[31mError:\033[0m failed to create directory: ~/.config/LogDoctor"
		exit 1
	fi
fi
sudo chmod 644 ./logdoctor.conf
cp -fp ./logdoctor.conf ~/.config/LogDoctor
if [[ "$?" != "0" ]]
then
	echo -e "\033[31mError:\033[0m failed to copy configuration file"
	exit 1
fi


if [ ! -d /usr/share/LogDoctor ]
then
	sudo mkdir -p /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo -e "\033[31mError:\033[0m failed to create directory: /usr/share/LogDoctor"
		exit 1
	fi
fi
sudo chmod -R 644 ./logdocdata/help/*
sudo chmod -R 444 ./logdocdata/licenses/*
which rsync &> /dev/null
if [[ "$?" == "0" ]]
then
	sudo rsync -r --delete ./logdocdata /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo -e "\033[31mError:\033[0m failed to copy LogDoctor's data"
		exit 1
	fi
else
	sudo cp -rfp ./logdocdata/{help,licenses} /usr/share/LogDoctor/
	if [[ "$?" != "0" ]]
	then
		echo -e "\033[31mError:\033[0m failed to copy LogDoctor's data"
		exit 1
	fi
fi


sudo chmod 644 ./LogDoctor.svg
sudo cp -fp ./LogDoctor.svg /usr/share/LogDoctor/
if [[ "$?" != "0" ]]
then
	echo -e "\033[31mError:\033[0m failed to copy LogDoctor's icon"
	exit 1
fi


sudo chmod 644 ./LogDoctor.desktop
sudo cp -fp ./LogDoctor.desktop /usr/local/share/applications/
if [[ "$?" != "0" ]]
then
	echo -e "\033[31mError:\033[0m failed to create a menu entry"
	exit 1
fi


cd ../build
sudo chmod 755 ./logdoctor
sudo cp -fp ./logdoctor /usr/bin/
if [[ "$?" != "0" ]]
then
	echo -e "\033[31mError:\033[0m failed to copy the executable"
	exit 1
fi


# Installation finished
echo -e "\033[92m-->\033[0m \033[1mInstalled succesfully\033[0m"
cd "$current_path"
