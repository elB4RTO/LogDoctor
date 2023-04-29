#!/bin/bash

# Remove the app bundle
if [ -e /Applications/LogDoctor.app ]
then
	sudo rm -r /Applications/LogDoctor.app
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove the app bundle: /Applications/LogDoctor"
	fi
else
	echo "Warning: app bundle not found"
fi

# Remove the configurations file
if [ -e ~/Lybrary/Preferences/LogDoctor ]
then
	rm -r ~/Lybrary/Preferences/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove the configuration: "~/Lybrary/Preferences/LogDoctor
	fi
else
	echo "Warning: configurations folder not found"
fi

# Remove app data
if [ -e ~/"Lybrary/Application Support/LogDoctor" ]
then
	rm -r ~/"Lybrary/Application Support/LogDoctor"
	if [[ "$?" != "0" ]]
	then
		echo "Error: failed to remove LogDoctor's data and databases: "~/Lybrary/"Application Support/LogDoctor"
	fi
else
	echo "Warning: LogDoctor's data and databases folder not found"
fi


# Uninstallation finished
echo "Uninstallation finished"
