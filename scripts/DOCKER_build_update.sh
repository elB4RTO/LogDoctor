#!/bin/bash

# Store the actual path
current_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(dirname $(realpath $0)))"
cd "$docdir"

# Check docker availability
which docker &> /dev/null
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) Docker is not installed"
	exit 1
fi

# Check the existence of a previous LogDoctor installation
if [ -e /usr/bin/logdoctor ]
then
	grep "docker run" /usr/bin/logdoctor
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 11)Warning:$(tput sgr0) a previous non-Docker installation exists, please $(tput bold)uninstall$(tput sgr0) it before to proceed"
		exit 0
	fi
fi

# Check the existence of a previous image
docker images | grep logdoctor &> /dev/null
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 11)Warning:$(tput sgr0) no previous image found, please run the $(tput bold)install$(tput sgr0) script instead"
	exit 0
fi

# Check debian:latest image
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Checking base image$(tput sgr0)"

# Check the existence of debian:latest image
pull_updates=1
docker images | grep debian | grep latest &> /dev/null
if [[ "$?" == "0" ]]
then
	while :
	do
		echo "A $(tput bold)debian:latest$(tput sgr0) image is already installed\nIt is recomended to build LogDoctor on top of an updated image\nPull the latest updates? [y/n] : "
		read agree
		case "$agree"
		in
			[yY] | [yY][eE][sS] )
				break
			;;
			[nN] | [nN][oO] )
				pull_updates=0
				break
			;;
			*)
				echo "$(tput setaf 11)Warning:$(tput sgr0) not a valid choice"
				sleep 1
			;;
		esac
	done
fi
if [[ "$pull_updates" == "1" ]]
then
	# Image not found, pull it
	docker pull debian:latest
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to pull debian:latest"
		exit 1
	fi
fi

# Done checking
wait
echo "$(tput setaf 10)-->$(tput sgr0) Checked succesfully"

# Remove the old image
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Removing old LogDoctor image$(tput sgr0)"

docker image rm --force logdoctor:latest
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove logdoctor:latest"
	exit 1
fi

# Done removing
wait
echo "$(tput setaf 10)-->$(tput sgr0) Removed succesfully"

# Start the build process
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Building new LogDoctor image$(tput sgr0)"

# Build the docker image
docker build -t logdoctor:latest .
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to build the image"
	exit 1
fi

# Compilation finished
wait
echo "$(tput setaf 10)-->$(tput sgr0) Built succesfully"

# Start updating LogDoctor
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Updating$(tput sgr0)"

cd installation_stuff/docker

chmod 644 ../LogDoctor.desktop
sudo install -DC ../LogDoctor.desktop -t /usr/share/applications
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to create a menu entry"
	exit 1
fi

chmod 755 ./logdoctor
sudo install -DC ./logdoctor /usr/bin/logdoctor
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to copy the executable"
	exit 1
fi

# Update finished
echo "$(tput setaf 10)-->$(tput sgr0) $(tput bold)Updated succesfully$(tput sgr0)"
cd "$current_path"
