#!/bin/bash


## Exit the script if any step encounters error
set -e

## Check if user wants to enable or disable logging of amdgpu
if [ "$#" -ne 1 ]; then
  echo "$0 requires input: 0 to disable, 1 to enable amdgpu driver logging"
  echo "Run script as follows:  $0 0 OR  $0 1"
fi


#
# Capture users log preference
#   ONE enables it while NON-ONE disables it
# 
# Option 'p' enables debug
# Option 'l' prints line number
# Option 'f' prints function name
# Option 't' prints ID of the thread
#
logFlag=$1
if [[ "$logFlag" -eq 1 ]]; then
  # Enable
  # echo 'module amdgpu +flpt' > /sys/kernel/debug/dynamic_debug/control
  sudo sh -c "echo 'module  amdgpu +flpt' > /sys/kernel/debug/dynamic_debug/control"
else
  # Disable
  # echo 'module amdgpu -flpt' > /sys/kernel/debug/dynamic_debug/control
  sudo sh -c "echo 'module  amdgpu -flpt' > /sys/kernel/debug/dynamic_debug/control"
fi

