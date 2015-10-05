#!/usr/bin/env sh

#**
#* @file    scripts/setup.sh
#* @brief   Development setup script
#* @version 0.1
#* @date    29.01.2015.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

# Installs pre-commit hook
ln -s ../../scripts/pre-commit.sh ./.git/hooks/pre-commit

# Installs needed packages

sudo apt-get install nasm
sudo apt-get install bochs bochs-x bochs-sdl
sudo apt-get install qemu qemu-kvm
sudo apt-get install texinfo
sudo apt-get install indent
