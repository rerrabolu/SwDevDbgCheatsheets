#!/bin/bash

# Exit the script if any step encounters error
set -e

# Print Help message
function printHelp() {
  echo -e ""
  echo -e "\t$0 requires module name as input"
  echo -e "\te.g.  $0 amdgpu"
  echo -e ""
  exit
}

# Check if user has given module name as argument
if [ "$#" -ne 1 ]; then
  printHelp
fi

#
# Print the name of kernel module whose parameters are
# to be queried
#
inp_mod_name=$1
echo "Printing parameters for kernel module: $1"

# Print the various Loaded Modules and their parameters
cat /proc/modules | cut -f 1 -d " " | while read module; do \
  if [ "$module" == "$inp_mod_name" ]; then \
      echo "Module: $module"; \
      if [ -d "/sys/module/$module/parameters" ]; then \
          ls /sys/module/$module/parameters/ | while read parameter; do \
          echo -n "Parameter: $parameter --> "; \
          cat /sys/module/$module/parameters/$parameter; \
          done; \
      fi; \
      echo; \
  fi; \
done

