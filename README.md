<div align="center">
  <h1>LogDoctor</h1>
  <p>Parse Apache2 / Nginx / IIS logs and view dynamically generated statistics</p>
  <br/>
  <img src="https://img.shields.io/badge/version-2.04-fff"/>
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue"/>
  <img src="https://img.shields.io/badge/Qt-5.15-blue"/>
  <br/>
  <img src="https://img.shields.io/badge/Linux-supported-31c653"/>
  <img src="https://img.shields.io/badge/BSD-supported-31c653"/>
  <img src="https://img.shields.io/badge/Windows-supported-31c653"/>
  <img src="https://img.shields.io/badge/Mac%20OS%20X-supported-31c653"/>
  <br/>
  <img src="https://github.com/elB4RTO/LogDoctor/actions/workflows/cmake.yml/badge.svg"/>
 
  <h2>🇬🇧 🇪🇸 🇫🇷 🇮🇹 🇯🇵 🇧🇷</h2>
</div>

<br/><br/>

## Table of contents

- [Overview](#overview)
- [Installation and usage](#installation-and-usage)
  - [Requirements / dependencies](#requirements--dependencies)
  - [Usage without installation](#usage-without-installation)
  - [Usage with installation](#usage-with-installation)
  - [How to compile](#how-to-compile)
- [Updates](#updates)
  - [Version check](#version-check)
  - [How to update](#how-to-update)
- [Before to start](#before-to-start)
- [Logs data](#logs-data)
  - [Storage](#storage)
  - [Examined fields](#examined-fields)
  - [Logs options](#logs-options)
    - [Usage control](#usage-control)
    - [Logs path](#logs-path)
    - [Logs format](#logs-format)
      - [Apache2](#apache2)
      - [Nginx](#nginx)
      - [IIS](#iis)
    - [Blacklist](#blacklist)
    - [Warnlist](#warnlist)
- [Statistics](#statistics)
  - [Warnings](#warnings)
  - [Counts](#counts)
  - [Speed](#speed)
  - [Time of day](#time-of-day)
  - [Relational](#relational)
- [Extra features](#extra-features)
  - [Log files viewer](#log-files-viewer)
  - [Block note](#block-note)
  - [Games](#games)
- [Final considerations](#final-considerations)
  - [Backups](#backups)
  - [Estimated working speed](#estimated-working-speed)
- [Languages](#languages)
- [Contributions](#contributions)
  - [Translations](#translations)

<br/><br/>

## Overview

LogDoctor is a web servers' access logs parser which allows to view dynamic satistics of the collected data.<br/>
Supported web servers are **Apache2**, **Nginx** and **IIS**.

<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/log_files.png)

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/make_stats.png)

<br/><br/>

LogDoctor is a hard fork of [Craplog](https://github.com/elB4RTO/CRAPLOG).

<br/>

## Installation and usage

### Requirements / Dependencies

- **From binary**:
  - C++ 17
  - Qt5 *(Framework 5.15+, Linguist, Widgets, Charts, Sql, Network)*<br/><br/>
- **From source**:
  - *all the above*
  - Cmake
  - g++ / gcc / clang<br/><br/>

<br/>

### Usage without installation

- Download a pre-compiled [Release](https://github.com/elB4RTO/LogDoctor/releases)
  <br/>*or*<br/>
  Follow the step-by-step "[How to compile](#how-to-compile)" guide

- Run the executable

<br/>

### Usage with installation

#### From source

- Download and unzip this repo
  <br/>*or*<br/>
  `git clone https://github.com/elB4RTO/LogDoctor`<br/><br/>
- Step inside inside "*LogDoctor-main*"
  <br/>*or*<br/>
  `cd LogDoctor`<br/><br/>
- Run the installation script
  - Linux/BSD:
    - `chmod +x ./build_install.sh`
    - `./build_install.sh`
  - Windows:
    - run `WIN_build_install_1.bat` as normal user
    - right-click on `WIN_build_install_2.bat` and select **Run as Administrator**
  - Mac OS:
    - `chmod +x ./MAC_build_install.sh`
    - `./MAC_build_install.sh`

<br/>

#### From package

##### Arch-based distributions

- Pre-made package:
  - `wget https://github.com/elB4RTO/LogDoctor/archive/refs/tags/logdoctor-<VERSION>-x86_64.pkg.tar.zst`
  - `sudo pacman -U logdoctor-<VERSION>-x86_64.pkg.tar.zst`<br/><br/>
- From the AUR:
  - Using **yay**:
    - `yay -S logdoctor`<br/>
  - Manually:
    - `git clone https://aur.archlinux.org/logdoctor.git`
    - `cd logdoctor`
    - `makepkg -sci`<br/><br/>

##### Debian-based distributions

- Pre-made package:
  - `wget https://github.com/elB4RTO/LogDoctor/archive/refs/tags/logdoctor_<VERSION>_amd64.deb`
  - `sudo apt install ./logdoctor_<VERSION>_amd64.deb`

<br/>

#### From binary

- Download a pre-compiled [Release](https://github.com/elB4RTO/LogDoctor/releases)
- Run the installation executable, or the installation script if you prefer it

<br/><br/>

### How to compile

- Install the *dependencies* you're missing (usually the list reduces to just *Qt*, and *cmake* at least).<br/><br/>
- Download and unzip this repo
  <br/>*or*<br/>
  `git clone https://github.com/elB4RTO/LogDoctor`<br/><br/>
- Open a terminal inside "*LogDoctor-main/*"
  <br/>*or*<br/>
  `cd LogDoctor/`<br/><br/>
- Prepare a build folder:
  <br/>`mkdir build && cd build`<br/><br/>
- Prepare **Cmake**'s build files:
  <br/>`cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel`<br/><br/>
- Use **Cmake** to compile the entire project:
  <br/>`cmake --build ./ --target all`<br/><br/>
  If compilation fails, use the following command before to rebuild:
  <br/>`cmake --build ./ --target clean`
  <br/><br/>

#### Additional steps

- **Linux**:
  - No additional steps. Once compiling is done, you can move the executable file wherever you want and execute it from there.<br/><br/>

- **Windows**:
  - Create a new folder and move the executable in it:
    <br/>`mkdir LogDoctor && move LogDoctor.exe .\LogDoctor`<br/><br/>
  - Add the needed libraries to the executable's folder:<br/>
    - You need to know the path of your Qt installation, default is *C:\Qt*<br/>
    - You need to know which compiler you used, usually *MinGW*
      <br/>`cd C:\<path>\<to>\Qt\<version>\<compiler>\bin`<br/><br/>
    - Deploy Qt's libraries using Qt's additional tool:
      <br/>`windeployqt.exe C:\<path>\<to>\<LogDoctor>`<br/><br/>
    - Deploy C++ libraries by copying them:
      <br/>`copy "libstdc++-6.dll" C:\<path>\<to>\<LogDoctor>`
      <br/>`copy "libwinpthread-1.dll" C:\<path>\<to>\<LogDoctor>`
      <br/>`copy "libgcc_s_seh-1.dll" C:\<path>\<to>\<LogDoctor>`<br/><br/>
  - You can now move the executable's folder wherever you want and execute LogDoctor from there.<br/><br/>

- **Mac OS**:
  - No additional steps. Once compiling is done, you can use the app bundle to execute LogDoctor.

<br/><br/>

## Updates

### Version check

A version check utility is available while running LogDoctor to check the availability of a new version.<br/>
To check for updates, open the menu `Utilities`→`Version check`.

<br/>

### How to update

At the moment of writing, the only supported method is the manual update.<br/><br/>

#### From source

- Download and unzip this repo
  <br/>*or*<br/>
  `git clone https://github.com/elB4RTO/LogDoctor`<br/><br/>
- Step inside inside "*LogDoctor-main*"
  <br/>*or*<br/>
  `cd LogDoctor`<br/><br/>
- Run the update script
  - Linux/BSD:
    - `chmod +x ./build_update.sh`
    - `./build_update.sh`
  - Windows:
    - run `WIN_build_update_1.bat` as normal user
    - right-click on `WIN_build_update_2.bat` and select **Run as Administrator**
  - Mac OS:
    - `chmod +x ./MAC_build_update.sh`
    - `./MAC_build_update.sh`

<br/>

#### From package

Follow the same process as for installing

<br/>

#### From binary

- Download a pre-compiled [Release](https://github.com/elB4RTO/LogDoctor/releases)<br/>
- Run the update script

<br/><br/><br/>

## Before to start

When you run LogDoctor for the first time, please take a minute to set-up the things it needs.<br/>
Head to the **configurations** section and give a look at least at the [logs format](#logs-format) settings. You have to tell the doctor what he'll be dealing with!

<br/><br/>

## Logs data

Archived (**gzipped**) log files can be used as well as normal files.

<br/>

### Storage

Parsed data will be stored in an [SQLite](https://www.sqlite.org/about.html) database, which makes it easy to transport/view/edit it as you please.<br/>
If LogDoctor's funcionalities aren't enough for your needs, you can always use a *DB manager* or the SQLite *API* to make your own queries and retrieve the data you need.

<br/>

### Examined fields

Not all the available log fields (expecially for *Apache2* and *Nginx*) are taken into consideration.<br/>
The considered fields are:
- **Date** and **Time**
- Request stuff: **Protocol**, **Method**, **URI** and **Query**
- Server stuff: **Bytes received**, **Bytes sent** and **Time taken**
- Client stuff: **User-agent**, **IP address**, **Cookie** and **Referrer site**

Further informations can be found in the [wiki](https://github.com/elB4RTO/LogDoctor/wiki/Examined-fields) or while running LogDoctor.

<br/>

### Logs options

Various options can be configured about log files.

<br/>

#### Usage control

When you parse a file, it will be hashed using the **SHA256** algorithm and the hash will be stored in another database, to keep track of which files you've already used and help you not parsing them twice.<br/>

##### Note

If you don't know, *SHA256* produces an irreversible hash, which means that no information about the file can be retrieved from the hash.<br/>
You have full control on the hashes database (same as for the logs-data database): you can move, delete, view or edit it the way you want (but you must keep the original file-name).<br/>
LogDoctor will **never** grab and/or use any information about you or the usage you make of it.

<br/>

#### Logs path

A different logs path can be used for any of the three supported *Web Servers*.<br/>
It can be the default system folder or any folder you decide to use, just set it in the options.

<br/>

#### Logs format

Before to start parsing logs, you must set-up the *log format* that LogDoctor will have to use.<br/>
Head to the **configurations** section, tap `Logs`, select the **Web Server** you want to configure and tap `Format`.<br/>
Once inside the **Format** section, you can insert the *log format string* you're using. Don't forget to use the `Generete preview` button to generate a *log line sample* and **check the correctness** of the format!<br/>

For reliability reasons, LogDoctor **does not** support the usage of the **Carriage Return** inside the log format string.

<br/>

##### Apache2

The log format string must be specified. Any format is supported, if valid.<br/>
To retrieve your format string:
- open the configuration file `/etc/apache2/apache2.conf`
- *usually*, the line you're looking for is the one starting with `LogFormat` and ending with `combined`. It should be somewhere near to the end of the file.
- you must not paste the whole line, just the part holding the *format string*.<br/>
  Example:<br/>
  - this is the whole line:<br/>
    ```
    LogFormat "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"" combined
    ```
  - this is the *format string*:<br/>
    ```
    %h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"
    ```
    please notice that you have to remove the enclosing quotes/apostrophes as well<br/>

More informations can be found in the [wiki](https://github.com/elB4RTO/LogDoctor/wiki/Apache2) or while setting the format.

<br/>

##### Nginx

The log format string must be specified. Any format is supported, if valid.<br/>
To retrieve your format string:<br/>
- open the configuration file `/usr/local/etc/nginx/nginx.conf`
- *usually*, the line you're looking for is the one starting with `log_format main`. It should be somwehere in the middle of the file
- one **important** thing: don't paste the indentations and new lines! The default line is usualy declared in consecutive lines, and indented. You must reduce it to a one consecutive string (by also removing the *apostrophes* in the middle of it). The best way is to do this job inside the configuration file, then save and restart Nginx to see if any error is thrown.<br/>
  Example:
  - this is the whole line:<br/>
    ```
    log_format main '$remote_addr - $remote_user [$time_local] '
                    '"$request" $status $body_bytes_sent '
                    '"$http_referer" "$http_user_agent" "$gzip_ratio"';
    ```
  - this is the resulting *format string*:<br/>
    ```
    $remote_addr - $remote_user [$time_local] "$request" $status $bytes_sent "$http_referer" "$http_user_agent" "$gzip_ratio"
    ```
    please notice that you have to remove the enclosing apostrophes/quotes as well<br/>

More informations can be found in the [wiki](https://github.com/elB4RTO/LogDoctor/wiki/Nginx) or while setting the format.

<br/>

##### IIS

Supported log formats are: **W3C**, **NCSA** and **IIS**.<br/>

The *NCSA* and *IIS* modules doesn't allow any modification from the user, so nothing more have to be specified.

The *W3C* module instead allows the user to decide which fields to log, and thus you must declare the *log format string* you're using.
To retrieve your format string (for the *W3C* module only):
- open any of the log files which have been generated by this module
- the line you're looking for is the one starting with `#Fields:`, usually at the beginning of the file.<br/>
  Example:<br/>
  - this is the whole line:<br/>
    ```
    #Fields: date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken
    ```
  - this is the *format string*:<br/>
    ```
    date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken
    ```

More informations can be found in the [wiki](https://github.com/elB4RTO/LogDoctor/wiki/IIS) or while setting the format.

<br/><br/>

#### Blacklist

You can add elements to the **blacklist** to avoid storing the lines containing those elements.

Each web server has its own list.

<br/>

#### Warnlist

As for the *blacklist*, you can add elements to the **warnlist**.<br/>
*Warnlists* will mark with a **warning** the lines triggering them. Warnings can be viewed and modified in the relative [statistics](#warnings) section.

Each web server has its own lists.

<br/>

## Statistics

Most of the *statistics sections* allows you to set filters to the log fields, to skim data by only including lines matching those parameters.<br/>

<br/>

### Warnings

In the *warning* section you can view the lines which triggered a warning, as well as remove any of the warnings and/or add your own.<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/stats_warnings.png)

<br/>

### Speed

In the *speed* section you can view how fast has been your server at serving contents (if you logged the *time taken*, of course).<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/stats_speed.png)

<br/>

### Counts

The *count* section is very simple. It just shows the recurrence of the elements for a specific field.<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/stats_count.png)

<br/>

### Time of day

In the *time of day* section you can see the traffic, in terms of number of requests logged.<br/>
When viewing a period of time, the mean value (of all the logged days in that period) is shown.<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/stats_daytime.png)

<br/>

### Relational

In the *relational* section you can view how many times a specific field brought to another.<br/>
This section is more suited for long periods of time.<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/stats_relational.png)

<br/>

### Globals

In the *globals* section you can have an overview of your logs history.<br/>

![screenshot](https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/stats_globals.png)

<br/><br/>

## Extra features

### Log files viewer

Use the built-in logs viewer to inspect the content of your log files.<br/>
Color schemes will be applied using the currently set log format.

<br/>

### Block-note

A block-note utility is available at `Tools`→`BlockNote` which can be used to temporary write text, notes, etc.

<br/>

### Games

Simple mini-games to pass the time.<br/><br/>

#### CrissCross

<img height="300px" src="https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/game_crisscross.png" /><br/><br/>

#### Snake

<img height="350px" src="https://raw.githubusercontent.com/elB4RTO/screenshots/main/LogDoctor/game_snake.png" /><br/><br/>

<br/><br/>

## Final considerations

### Backups

LogDoctor can automatically do a backup of your **logs database** file, so you can recover your data in case something goes wrong.<br/>

Move inside LogDoctor's folder (if you don't know/remember the path, open the `Utilities`→`Infos`>`Paths` menu to view it) and open the folder named "**backups**'.<br/>
Here you will find the backups with an increasing index, where '.1' represents the newest.

A new backup is made every time you quit LogDoctor after doing a job which affected the database in any way.

#### Note

Only the *logs-data database* will be backed-up, the *hashes database* **won't**.<br/>
This is because it is unlikely (supposedly impossible) that a hash equals another, therefore they're supposed to be useful for a short period of time (that is, since you or your web server delete the original log files).

<br/>

### Estimated working speed

1~15 MB/s

May be higher or lower depending on the complexity of the logs, the complexity of the blacklist/warnlists, your hardware and the workload of your system during the execution.

<br/><br/>

## Languages

LogDoctor is available in:

- 🇬🇧 **English** (100%)

- 🇮🇹 **Italian** (90%, *wanna [contribute](#translations)?*)

- 🇪🇸 **Spanish** (90%, *wanna [contribute](#translations)?*)

- 🇫🇷 **French** (90%, *wanna [contribute](#translations)?*)

- 🇧🇷 **Portuguese** [**Brazil**] (90%, *wanna [contribute](#translations)?*)

- 🇯🇵 **Japanese** (90%, *wanna [contribute](#translations)?*)

<br/><br/>

## Contributions

LogDoctor is under development.

If you have suggestions about how to improve it, please open an ![issue](https://github.com/elB4RTO/LogDoctor/issues).

If you want to contribute to the code, please read the [Contribution Guidelines](https://github.com/elB4RTO/LogDoctor/blob/main/CONTRIBUTING.md).

If you want to contribute to the translation, please read the [Translation Guidelines](#how-to-contribute-to-translations).

<br/>

### Translations

Current translations under developement:

- `it_IT` : **90%**

- `es_ES` : **90%** *(auditor needed)*

- `fr_FR` : **90%** *(auditor needed)*

- `pt_BR` : **90%** *(auditor needed)*

- `ja_JP` : **90%** *(auditor needed)*

If you have a request for a missing language or you're willing to contribute, please refer to [this issue](https://github.com/elB4RTO/LogDoctor/issues/10).

<br/>

#### How to contribute to translations

Since the whole application is build upon Qt, translations are made throught `.ts` [translation files](https://github.com/elB4RTO/LogDoctor/tree/main/logdoctor/translations).

<br/>

The easiest way to go is to use **Qt Linguist**:
- Download or clone this repo<br/>
- Open the `.ts` translation file of your language using QtLinguist<br/>
- Translate (*don't know how? Follow [this video](https://www.youtube.com/watch?v=GNyfkuDchNQ)*)<br/>
- **Push only the `.ts` files, you don't have to release them**: pull request containing `.qm` binaries won't be accepted.<br/>

<br/><br/>

If you don't want to install QtLinguist, you can do it the hard way, by opening the files with a text editor and do it manually:
- Here is a sample of some text waiting for translation<br/>
  You can see the original text enclosed in the `<source>` tags. Don't edit it.<br/>
  ```
      <message>
          <location filename="..." line="n"/>
          <source>This is the original text</source>
      </message>
  ```
- Everything you need to do is to add the missing line containing the translated text, without editing the other lines.<br/>
  Here is a sample after having translated it<br/>
  ```
      <message>
          <location filename="..." line="n"/>
          <source>Original text</source>
          <translation type="unfinished">Translated text goes here</translation>
      </message>
  ```

<br/>

If you don't feel comfortable with any of the above solutions, please open an ![issue](https://github.com/elB4RTO/LogDoctor/issues) and write your translations/corrections there, in a clear way.

<br/>

#### Translation guidelines

*[hints from Qt](https://doc.qt.io/qt-6/linguist-overview.html)*

Just follow some simple guidelines to ensure a correct and clear contribution:

- Respect the structure of the phrase: if *(for example)* it starts or ends with a whitespace or something, please do the same.

- Please leave your translations marked as *unfinished*, they will be checked and un-marked after having been verified.

- If you see a translation marked as *finished*, please edit it only if you're sure that your definition suits better.

<br/>
