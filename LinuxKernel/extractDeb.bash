#!/bin/bash

# Exit the script if any step encounters error
set -e

# Print Help message
function printHelp() {
	echo -e ""
	echo -e "\t$0 requires two arguments"
	echo -e "\te.g. $0 <pkg-name.rpm>   <directory>"
	echo -e "\te.g. $0 kernel-ml-devel-6.6.8-1.el9.elrepo.x86_64.rpm $HOME/tmp "
	echo -e ""
	exit
}

rpmPkg=$1
xtrctDir=$2

echo -e "\tExtracting $rpmPkg to $xtrctDir "
rpm2cpio  ${rpmPkg} | cpio -idmv -D ${xtrctDir}



