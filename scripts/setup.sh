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
