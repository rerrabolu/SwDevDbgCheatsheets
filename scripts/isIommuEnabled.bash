#!/bin/bash

#
# Exit in case of error
#
set -e

# Function to determine if a directory is empty or not
function is_empty {
    local dir="$1"
    shopt -s nullglob
    local files=( "$dir"/* "$dir"/.* )
    [[ ${#files[@]} -eq 2 ]]
}

if is_empty /sys/kernel/iommu_groups/; then
  echo "IOMMU Groups folder is empty";
  echo "IOMMU is enabled";
else
  echo "IOMMU Groups folder is not empty";
  echo "IOMMU is disabled";
fi

#
# Loop through /sys/kernel/iommu_groups folder
# printing details of various PCIe devices
#
# @note: Will print nothing if IOMMU is not enabled
#
shopt -s nullglob
for g in /sys/kernel/iommu_groups/*; do
    echo "IOMMU Group ${g##*/}:"
    for d in $g/devices/*; do
        echo -e "\t$(lspci -nns ${d##*/})"
    done;
done;



