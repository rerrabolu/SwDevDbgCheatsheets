#!/bin/bash

# Exit the script if any step encounters error
set -e


#
# Iterate through the list of IOMMU groups on system
#
for d in /sys/kernel/iommu_groups/*/devices/*;
    do
      n=${d#*/iommu_groups/*};
      n=${n%%/*};
      printf 'IOMMU Group %s ' "$n";
      lspci -nns "${d##*/}";
    done;

