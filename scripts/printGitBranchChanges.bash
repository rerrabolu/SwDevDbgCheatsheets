#!/bin/bash

#
# Save the current working directory before changing
# to Hsa Workspace
#
currDir=${PWD}

# List of Valid Branches. Remove the first two columns
# from command output
#
validList=$(git branch | cut -c 3-)

validListXXX=(
                "-q"
                "-t"
               
                "-s 5 -d 3"
                "-s 5 -d 4"

            );

#
# Execute Valid List Configurations
#
# for val in "${validListXXX[@]}"
for val in ${validList[@]}
  do
    echo "Executing showList on branch: $val"
    git checkout $val
    git diff --name-only HEAD HEAD~${1}
    echo " "
  done

cd ${currDir}

echo "Completed executing showList on all git branches of Repo."
echo ""

