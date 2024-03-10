#!/bin/bash

# Exit the script if any step encounters error
set -e

# Print Help message
function printHelp() {
	echo -e ""
	echo -e "\t$0 cannot report HW IP_VERSION of GPU"
	echo -e ""
	exit
}


#
# Bind the path of file that encodes HW IP_VERSION
#

#
# Determine if the platform supports reporting of Hardware
# IP_VERSION. If it is supported then capture the path prefix
# to use in getting the IP_VERSION of GPU devices
#
# @note: Assumes that all of the AMD GPU devices present
# on the platform either support or do not support the
# discovery of HW IP_VERSION. This script will report
# incorrectly if one or more of the devices do not support
#
# @note: Script will print a helpful message if the platform
# does not support discovery of IP_VERSION of its AMD GPU devices
#
prefixPathList=($( find /sys -name ip_discovery ))
numGpus="${#prefixPathList[@]}"
if [ "$numGpus" -eq 0 ]; then
	printHelp
fi

#
# Define the path to suffix to each element found from above
# discovery / binding
#
suffixPath="die/0/GC/0/"
# echo -e "Value of suffixPath: $suffixPath"

#
# Define the filenames that encode the Major, Minor and
# Stepping versions of Hardware IP
#
fileList=("major" "minor" "revision");
 
#
# Print the HW IP_VERSION of all of the AMD GPU devices
#
echo -e " "
for (( idx = 0; idx < $numGpus; idx++ ))
	do
		fullPath="${prefixPathList[idx]}/${suffixPath}/"
		# echo -e "Printing the Full Path: ${fullPath}"
		majorVer=`cat ${fullPath}/${fileList[0]}`
		minorVer=`cat ${fullPath}/${fileList[1]}`
		stepVer=`cat ${fullPath}/${fileList[2]}`
		echo -e "${idx}: HW IP_VERSION is: ( $majorVer, $minorVer, $stepVer ) "
	echo -e " "
done
echo -e " "


