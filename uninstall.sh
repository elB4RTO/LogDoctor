#!/bin/bash

# Start uninstalling
echo "$(tput setaf 12)==>$(tput sgr0) $(tput bold)Uninstalling$(tput sgr0)"

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

# Remove LogDoctor's data
if [ -e /usr/share/LogDoctor ]
then
	sudo rm -r /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove LogDoctor's data: $(tput setaf 11)/usr/share/LogDoctor$(tput sgr0)"
	fi
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) LogDoctor's data folder not found"
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

# Ask about removing databases
if [ -f ~/.config/LogDoctor/logdoctor.conf ]
then
	db_data_path=$(grep DatabaseDataPath ~/.config/LogDoctor/logdoctor.conf | cut -d\= -f2)
	db_hashes_path=$(grep DatabaseHashesPath ~/.config/LogDoctor/logdoctor.conf | cut -d\= -f2)
	if [ -e "$db_data_path" ] | [ -e "$db_hashes_path" ]
	then
		while :
		do
			printf "Remove the databases? [y/n] : "
			read rem
			case $rem
			in
				"y"|"Y"|[yY][eE][sS])
					# Remove the data collection database
					if [ -e "$db_data_path/collection.db" ]
					then
						rm "$db_data_path/collection.db"
						if [[ "$?" != "0" ]]
						then
							echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove data collection database: $(tput setaf 11)$db_data_path/collection.db$(tput sgr0)"
						fi
						if [ -d "$db_data_path/backups" ]
						then
							rm -r "$db_data_path/backups"
							if [[ "$?" != "0" ]]
							then
								echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove data collection database backups: $(tput setaf 11)$db_data_path/backups$(tput sgr0)"
							fi
						else
							echo "$(tput setaf 11)Warning:$(tput sgr0) data collection database's backups folder not found"
						fi
					else
						echo "$(tput setaf 11)Warning:$(tput sgr0) data collection database not found"
					fi
					# Remove the hashes database
					if [ -e "$db_hashes_path/hashes.db" ]
					then
						rm "$db_hashes_path/hashes.db"
						if [[ "$?" != "0" ]]
						then
							echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove hashes database: $(tput setaf 11)$db_hashes_path/hashes.db$(tput sgr0)"
						fi
					else
						echo "$(tput setaf 11)Warning:$(tput sgr0) hashes database not found"
					fi
					# Wipe the default folder
					if [ -e ~/.local/share/LogDoctor ]
					then
						rm -r ~/.local/share/LogDoctor
						if [[ "$?" != "0" ]]
						then
							echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove the default databases folder: $(tput setaf 11)"~/.local/share/LogDoctor"$(tput sgr0)"
						fi
					else
						echo "$(tput setaf 11)Warning:$(tput sgr0) default databases' folder not found"
					fi
					break
				;;
				"n"|"N"|[nN][oO])
					break
				;;
				*)
					echo "Invalid answer"
				;;
			esac
		done
	fi
fi

# Ask about removing the configuration file
if [ -e ~/.config/LogDoctor ]
then
	while :
	do
		printf "Remove the configuration file? [y/n] : "
		read rem
		case $rem
		in
			"y"|"Y"|[yY][eE][sS])
				rm -r ~/.config/LogDoctor
				if [[ "$?" != "0" ]]
				then
					echo "$(tput setaf 1)Error:$(tput sgr0) failed to remove the configuration: $(tput setaf 11)"~/.config/LogDoctor"$(tput sgr0)"
				fi
			;;
			"n"|"N"|[nN][oO])
				break
			;;
			*)
				echo "Invalid answer"
			;;
		esac
	done
else
	echo "$(tput setaf 11)Warning:$(tput sgr0) configurations' folder not found"
fi


# Uninstallation finished
echo "$(tput setaf 10)-->$(tput sgr0) $(tput bold)Uninstallation finished$(tput sgr0)"
