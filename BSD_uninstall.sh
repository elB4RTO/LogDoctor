#!/bin/bash

# Start uninstalling
echo "\033[94m==>\033[0m \033[1mUninstalling\033[0m"

# Remove the executable
if [ -e /usr/bin/logdoctor ]
then
	sudo rm /usr/bin/logdoctor
	if [[ "$?" != "0" ]]
	then
		echo "\033[31mError:\033[0m failed to remove the executable: \033[93m/usr/bin/logdoctor\033[0m"
	fi
else
	echo "\033[93mWarning:\033[0m executable not found"
fi

# Remove LogDoctor's data
if [ -e /usr/share/LogDoctor ]
then
	sudo rm -r /usr/share/LogDoctor
	if [[ "$?" != "0" ]]
	then
		echo "\033[31mError:\033[0m failed to remove LogDoctor's data: \033[93m/usr/share/LogDoctor\033[0m"
	fi
else
	echo "\033[93mWarning:\033[0m LogDoctor's data folder not found"
fi

# Remove the menu entry
if [ -e /usr/share/applications/LogDoctor.desktop ]
then
	sudo rm /usr/share/applications/LogDoctor.desktop
	if [[ "$?" != "0" ]]
	then
		echo "\033[31mError:\033[0m failed to remove the menu entry: \033[93m/usr/share/applications/LogDoctor.desktop\033[0m"
	fi
else
	echo "\033[93mWarning:\033[0m menu entry not found"
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
							echo "\033[31mError:\033[0m failed to remove data collection database: \033[93m$db_data_path/collection.db\033[0m"
						fi
						if [ -d "$db_data_path/backups" ]
						then
							rm -r "$db_data_path/backups"
							if [[ "$?" != "0" ]]
							then
								echo "\033[31mError:\033[0m failed to remove data collection database backups: \033[93m$db_data_path/backups\033[0m"
							fi
						else
							echo "\033[93mWarning:\033[0m data collection database's backups folder not found"
						fi
					else
						echo "\033[93mWarning:\033[0m data collection database not found"
					fi
					# Remove the hashes database
					if [ -e "$db_hashes_path/hashes.db" ]
					then
						rm "$db_hashes_path/hashes.db"
						if [[ "$?" != "0" ]]
						then
							echo "\033[31mError:\033[0m failed to remove hashes database: \033[93m$db_hashes_path/hashes.db\033[0m"
						fi
					else
						echo "\033[93mWarning:\033[0m hashes database not found"
					fi
					# Wipe the default folder
					if [ -e ~/.local/share/LogDoctor ]
					then
						rm -r ~/.local/share/LogDoctor
						if [[ "$?" != "0" ]]
						then
							echo "\033[31mError:\033[0m failed to remove the default databases folder: \033[93m"~/.local/share/LogDoctor"\033[0m"
						fi
					else
						echo "\033[93mWarning:\033[0m default databases' folder not found"
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
					echo "\033[31mError:\033[0m failed to remove the configuration: \033[93m"~/.config/LogDoctor"\033[0m"
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
else
	echo "\033[93mWarning:\033[0m configurations' folder not found"
fi


# Uninstallation finished
echo "\033[92m-->\033[0m \033[1mUninstallation finished\033[0m"
