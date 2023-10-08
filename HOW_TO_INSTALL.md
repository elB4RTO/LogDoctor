# How to install

<br/>

## From source

- Run the installation script
  - Linux:
    - `bash scripts/LINUX_build_install.sh`
  - BSD:
    - `bash scripts/BSD_build_install.sh`
  - Windows:
    - step inside the *scripts* folder
    - run `WIN_build_install_1.bat` as normal user
    - right-click on `WIN_build_install_2.bat` and select **Run as Administrator**
  - Mac OS:
    - `bash scripts/MAC_build_install.sh`

<br/><br/>

## From package

### Arch-based distributions

- Pre-made package:
  - `wget https://github.com/elB4RTO/LogDoctor/archive/refs/tags/logdoctor-<VERSION>-x86_64.pkg.tar.zst`
  - `sudo pacman -U logdoctor-<VERSION>-x86_64.pkg.tar.zst`<br/><br/>
- From the AUR:
  - Using **yay**:
    - `yay -S logdoctor`<br/>
  - Manually:
    - `git clone https://aur.archlinux.org/logdoctor.git -b master --depth 1`
    - `cd logdoctor`
    - `makepkg -sci`

<br/>

### Debian-based distributions

- Pre-made package:
  - `wget https://github.com/elB4RTO/LogDoctor/archive/refs/tags/logdoctor_<VERSION>_amd64.deb`
  - `sudo apt install ./logdoctor_<VERSION>_amd64.deb`

<br/><br/>

## From binary

- Download a pre-compiled [Release](https://github.com/elB4RTO/LogDoctor/releases)
- Run the installation executable or the installation script

<br/><br/>

## As Docker

- Run the installation script
  - Linux:
    - `bash scripts/DOCKER_build_install.sh`

<br/>
