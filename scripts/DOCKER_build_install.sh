#!/bin/bash

# Store the actual path
current_path=$(pwd)

# Get the path of LogDoctor-git's folder and move in
docdir="$(dirname $(dirname $(realpath $0)))"
cd "$docdir"

# Check the existence of a previous image
docker images | grep logdoctor &> /dev/null
if [[ "$?" == "0" ]]
then
	echo "$(tput setaf 11)Warning:$(tput sgr0) an image already exists, please run the $(tput bold)update$(tput sgr0) script instead"
	exit 0
fi

# Check docker availability
which docker &> /dev/null
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) Docker is not installed"
	exit 1
fi

# Check debian:latest image
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Checking base image$(tput sgr0)"

# Check the existence of debian:latest image
docker images | grep debian | grep latest &> /dev/null
if [[ "$?" != "0" ]]
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

# Start the build process
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Building LogDoctor image$(tput sgr0)"

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

cd installation_stuff/docker

# Create a volume to share data with the image
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Creating LogDoctor data volume$(tput sgr0)"

# Create the volume
docker create --name logdoctordata logdoctor
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to create volume"
	exit 1
fi

# Copy the needed data
function data_transfer_failed()
{
	echo "$(tput setaf 1)Error:$(tput sgr0) failed to transfer data to volume"
	docker rm logdoctordata
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove volume after failure"
		exit 1
	fi
	rmdir tmp
	exit 1
}
test -e tmp && rm -rf tmp
mkdir tmp
docker cp ./tmp logdoctordata:/root/.config
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
docker cp ./tmp logdoctordata:/root/.config/LogDoctor
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
docker cp ./logdoctor.conf logdoctordata:/root/.config/LogDoctor/logdoctor.conf
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
docker cp ./tmp logdoctordata:/root/.local
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
docker cp ./tmp logdoctordata:/root/.local/share
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
docker cp ./tmp logdoctordata:/root/.local/share/LogDoctor
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
docker cp ./tmp logdoctordata:/root/logs
if [[ "$?" != "0" ]]
then
	data_transfer_failed
fi
for folder in apache2 nginx iis
do
	docker cp ./tmp logdoctordata:/root/logs/$folder
	if [[ "$?" != "0" ]]
	then
		data_transfer_failed
	fi
done
rmdir tmp

# Creation finished
wait
echo "$(tput setaf 10)-->$(tput sgr0) Created succesfully"

# Start installing LogDoctor
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Installing$(tput sgr0)"

chmod 644 ../LogDoctor.desktop
sudo install -DC ./LogDoctor.desktop -t /usr/share/applications
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

# Installation finished
echo "$(tput setaf 10)-->$(tput sgr0) $(tput bold)Installed succesfully$(tput sgr0)"
cd "$current_path"
