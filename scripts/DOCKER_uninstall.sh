#!/bin/bash

# Check docker availability
which docker &> /dev/null
if [[ "$?" != "0" ]]
then
	echo "$(tput setaf 1)Error:$(tput sgr0) Docker is not installed"
	exit 1
fi

# Start uninstalling
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Uninstalling$(tput sgr0)"

# Check the existence of a previous image
docker images | grep logdoctor &> /dev/null
if [[ "$?" == "0" ]]
then
	docker image rm --force logdoctor:latest
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove $(tput bold)logdoctor:latest$(tput sgr0) image "
	fi
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) docker image $(tput bold)logdoctor:latest$(tput sgr0) not found"
fi

# Check the existence of a previous image
docker volume ls | grep logdoctordata &> /dev/null
if [[ "$?" == "0" ]]
then
	while :
	do
		echo "Remove $(tput bold)logdoctordata$(tput sgr0) volume? [y/n] : "
		read agree
		case "$agree"
		in
			[yY] | [yY][eE][sS] )
				docker volume rm --force logdoctordata
				if [[ "$?" != "0" ]]
				then
					echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove $(tput bold)logdoctordata$(tput sgr0) volume"
				fi
				break
			;;
			[nN] | [nN][oO] )
				break
			;;
			*)
				echo "$(tput setaf 11)Warning:$(tput sgr0) not a valid choice"
				sleep 1
			;;
		esac
	done
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) docker volume $(tput bold)logdoctordata$(tput sgr0) not found"
fi

# Check the existence of debian:latest image
docker images | grep debian | grep latest &> /dev/null
if [[ "$?" == "0" ]]
then
	while :
	do
		echo "Remove $(tput bold)debian:latest$(tput sgr0) image? [y/n] : "
		read agree
		case "$agree"
		in
			[yY] | [yY][eE][sS] )
				docker image rm --force debian:latest
				if [[ "$?" != "0" ]]
				then
					echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove $(tput bold)debian:latest$(tput sgr0) image"
				fi
				break
			;;
			[nN] | [nN][oO] )
				break
			;;
			*)
				echo "$(tput setaf 11)Warning:$(tput sgr0) not a valid choice"
				sleep 1
			;;
		esac
	done
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) docker image $(tput bold)debian:latest$(tput sgr0) not found"
fi

# Remove the executable
if [ -e /usr/bin/logdoctor ]
then
	sudo rm /usr/bin/logdoctor
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove the executable: $(tput setaf 11)/usr/bin/logdoctor$(tput sgr0)"
	fi
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) executable not found"
fi

# Remove the menu entry
if [ -e /usr/share/applications/LogDoctor.desktop ]
then
	sudo rm /usr/share/applications/LogDoctor.desktop
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove the menu entry: $(tput setaf 11)/usr/share/applications/LogDoctor.desktop$(tput sgr0)"
	fi
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) menu entry not found"
fi

# Uninstallation finished
echo "$(tput setaf 10)-->$(tput sgr0) $(tput bold)Uninstallation finished$(tput sgr0)"
