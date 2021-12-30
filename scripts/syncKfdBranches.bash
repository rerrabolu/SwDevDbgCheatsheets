#!/bin/bash

# Exit the script if any step encounters error
set -e

# Save current directory and cd into Linux KFD dir
currDir=${PWD}
cfgEnv="${HOME}/git/compute/build/envsetup.sh"
kfdDir="${HOME}/git/compute/kernel/drivers/gpu/drm/amd"
cd ${kfdDir}
source ${cfgEnv}

#
# List of branches to checkout and sync
#
branchList=(
  "amd-staging-drm-next"
  "amd-staging-dkms-5.13"
  "amd-mainline-dkms-5.13"
);

# Capture the name of current branch
currBranch=`git branch --show-current`

#
# Iterate through each git branch and sync it to
# the latest commit available on gerrit-git repo
#
for branch in "${branchList[@]}" ; do
  echo -e "\nChecking out $branch"
  git checkout $branch
  git pull --rebase
done

# Restore KFD to its original branch
git checkout $currBranch

# Restore working directory
cd ${currDir}
echo -e "\nCompleted syncing of KFD git branches"
echo ""
 
