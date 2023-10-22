# How to update

<br/>

*At the moment of writing, the only supported method is the manual update*

<br/><br/>

## From source

- Run the update script
  - Linux:
    - `bash LINUX_build_update.sh`
  - BSD:
    - `bash BSD_build_update.sh`
  - Windows:
    - run `WIN_build_update_1.bat` as normal user
    - right-click on `WIN_build_update_2.bat` and select **Run as Administrator**
  - Mac OS:
    - `bash MAC_build_update.sh`

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

- Download a pre-compiled [Release](https://github.com/elB4RTO/LogDoctor/releases)<br/>
- Run the update script

<br/><br/>

## As Docker

- Run the update script
  - Linux:
    - `bash scripts/DOCKER_build_update.sh`

<br/>
