#!/bin/bash

# Ensure user is passing valid values
if [ "$#" -ne 2 ]; then
  echo "$0 requires 2 args: inputLogFile followed by outputLogFile"
  echo "Run script as follows:  $0 /home/user/foo.txt  /home/user/boo.txt"
  exit
fi

#
# The list of files to TAG by CSCOPE is collected
# into a file called srcList.txt in current directory.
#
currDir=${PWD}

# List of tokens to search for
tokenList=(
      "amdgpu_amdkfd_gpuvm_alloc_memory_of_gpu"
      "amdgpu_amdkfd_gpuvm_map_memory_to_gpu"
      "amd_acquire"
      "amd_get_pages"
      "amd_get_page_size"
      "amd_dma_map"
      "amd_dma_unmap"
      "amd_put_pages"
      "amd_release"
      );

# Set this to the file you want to parse
logFile="$1"
outFile="$2"
touch $logFile
touch $outFile

while IFS= read -r line
  do

  # Check if line has a token of interest
  for token in "${tokenList[@]}"
  do
    if grep -q $token <<< $line
    then
        echo $line >> $outFile
    fi
  done

done < "$logFile"

#
# Change back to the original directory
#
cd $currDir
