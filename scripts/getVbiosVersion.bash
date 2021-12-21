#!/bin/bash

#
# The script tries to collect version of vBIOS present on the
# various GPU devices of the platform
#

# Bail out if any of the commands fail
set -e

#
# Capture the versions of ROCm stack modules ROCr & ROCk
#
#printf "\n"
#echo -e "\t List of GPU devices present on the platform"
#printf "\n"
#lspci -d0x1002: | grep -i 'display controller'

# Iterate through each of the GPU devices printing their vBIOS version
printf "\n"
gpuList=`lspci -d0x1002: | grep -i 'display controller' | awk '{print $1}'`
for gpuDev in $gpuList; do
	vbiosVer=`find /sys/devices | grep -i $gpuDev | grep -i vbios`
	echo -e "GPU: $gpuDev \t vBIOS: `cat $vbiosVer`"
done
printf "\n"


