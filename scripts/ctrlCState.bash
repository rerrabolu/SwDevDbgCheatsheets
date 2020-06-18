#!/bin/bash

#
# Save current working directory to be restored later
#
currDir=${PWD}

#
# Specify the name of executable to run
#
NameOfExe="Name of executable file including path"

#
# Open Cpu Latency Control file and set desired C-state level
#
cpuLatency=/dev/cpu_dma_latency
exec 33> $cpuLatency
echo 0 >&33

#
# Print run parameters before launching application
# @note: It is critical that the latency control file
# remain open during the time period executable is running
#
if [ "$#" -ne 0 ]; then
  echo "Running $NameOfExe with Params: $@"
fi
$NameOfExe "$@"

#
# Close Cpu Latency Control file
#
exec 33>&-

#
# Change back to the original directory
#
cd $currDir

