#!/usr/bin/bash


dir=$1
pkg=$2

# echo 'Usage: extractDebPkg <pkgName> <dirName>'
# echo "Directory is: $1"
# echo "Package   is: $2"

# Extract a debian package to a local folder to inspect its contents
echo " " >&2;
echo "Extraction directory: $PWD/$1"
mkdir $PWD/$1
echo "  Extracting package: $2"
dpkg -e $2 $1
echo "Completed Extraction"
echo " " >&2

