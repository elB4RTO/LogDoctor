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
	echo -e "\nError: Cmake is not installed"
	exit
fi

# Check the existence of a previous executable file
if [ -e /usr/bin/logdoctor ]
then
	while true;
	do
		echo -e "\nWarning: the file /usr/bin/logdoctor already exists"
		printf "If you choose to continue, the actual file will be overwritten\nContinue? [y/n] : "
		read agree
		case "$agree"
		in
			"y" | "Y" | [yY][eE][sS])
				printf "\n"
				break
			;;
			*)
				exit
			;;
		esac
	done
fi

# Start the compilation process
echo -e "\nStarting compilation process\n"

# Make a build folder
if [ -e build ]
then
	rm -r build
fi
mkdir build && cd build

# Prepare the cmake files
cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel
if [[ "$?" != "0" ]]
then
	# an error occured during preparation
	echo -e "\nError: failed to prepare cmake files"
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
	echo -e "\nError: failed to compile"
	exit
fi

# Compilation finished
wait
echo -e "\nCompilation finished"

# Start installing LogDoctor
echo -e "\nStarting installation process"
cd ../installation_stuff/

if [ -e ~/.config/LogDoctor ]
then
	if [ -e ~/.config/LogDoctor/logdoctor.conf ]
	then
		# A configuration file already exists
		while true;
		do
			echo -e "\nWarning: a configuration file already exists"
			printf "If you choose 'YES' the current file will be overwritten\nIf you choose 'NO' the current file will be kept\nOverwrite? [y/n] : "
			read agree
			case "$agree"
			in
				"y" | "Y" | [yY][eE][sS])
					printf "\n"
					cp --no-preserve=all ./logdoctor.conf ~/.config/LogDoctor/
					if [[ "$?" != "0" ]]
					then
						# an error occured during compilation
						echo -e "\nError: failed to copy configuration file"
						exit
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
	mkdir -p ~/.config/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo -e "\nError: failed to create directory: ~/.config/LogDoctor"
		exit
	fi
	cp --no-preserve=all ./logdoctor.conf ~/.config/LogDoctor/
	if [[ "$?" != "0" ]]
	then
		echo -e "\nError: failed to copy configuration file"
		exit
	fi
fi


if [ ! -e ~/.local/share/LogDoctor ]
then
	mkdir -p ~/.local/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo -e "\nError: failed to create directory: ~/.local/share/LogDoctor"
		exit
	fi
fi
for res in $(ls ./logdocdata)
do
	rm -r ~/.local/share/LogDoctor/$res
	if [[ "$?" != "0" ]]
	then
		echo -e "\nError: failed to remove old resources: ~/.local/share/LogDoctor/$res"
		exit
	fi
done
cp -r --no-preserve=all ./logdocdata/* ~/.local/share/LogDoctor/
if [[ "$?" != "0" ]]
then
	echo -e "\nError: failed to copy LogDoctor's data"
	exit
fi


cp --no-preserve=all ./LogDoctor.desktop ~/.local/share/applications/
if [[ "$?" != "0" ]]
then
	echo -e "\nError: failed to create a menu entry"
	exit
fi


sudo cp --no-preserve=all ./logdoctor.svg /usr/share/icons
if [[ "$?" != "0" ]]
then
	echo -e "\nError: failed to copy LogDoctor's icon"
	exit
fi


cd ../build
chmod 755 ./logdoctor
sudo mv ./logdoctor /usr/bin/
if [[ "$?" != "0" ]]
then
	echo -e "\nError: failed to copy the executable"
	exit
fi


# Installation finished
echo -e "\nInstallation finished"
cd "$actual_path"
