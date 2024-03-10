#!/bin/bash

# Exit the script if any step encounters error
# set -e

#
# Print Help message
#
function printHelp() {
	echo -e ""
	echo -e "\t$0 <valid-package-name>"
	echo -e ""
	exit
}

# Ensure user is passing valid input arguments
if [ "$#" -ne 1 ]; then
	printHelp
fi

#
# Pipe the output by one TaB
#
myTab() { eval "$@" |& sed "s/^/\t/" ; return "$PIPESTATUS" ; }

#
# List packages on system whose name has "$pkgName"
#
# pkgList=( $(rpm -qa --queryformat '%{NAME}\n' | grep -i $pkgName))
echo -e ""
pkgName=$1
pkgList=($(rpm -qa | grep -i $pkgName))
if [ "$?" -ne 0 ]; then
	echo -e "No packages are found - is the package name valid?"
	echo -e " "
	exit
fi

# Determine the number of packages
numPkgs="${#pkgList[@]}"
echo "Pak Cnt: $numPkgs"
if [ "$numPkgs" -eq 0 ]; then
	echo -e "Zero packages are found - is the package name valid?"
	echo -e " "
	exit
fi

#
# Print the properties of desired RPMs
#
echo -e " "
echo -e "Number of Packages on System: $((numPkgs+1))"
echo -e " "
for (( idx = 0; idx < $numPkgs; idx++ ))
	do
		echo -e " "
		echo -e "$((idx+1)). Name Of Package: ${pkgList[idx]}"
		echo -e " "
		#rpm -qi ${pkgList[idx]}
		rpmInfo=`myTab rpm -qi ${pkgList[idx]}`
		echo -e "$rpmInfo"
		echo -e " "
	done
echo -e " "

