#!/bin/bash

#
# The script extracts a subst of lines of a file. This can be used
# to collect relevant statements of a log file for further analysis.
# 
# The idea is to use the begin and end lines of kernel log there by
# representing debug and print statements of the kernel
#
# @note: Depending upon which file is used to extract the script
# might require sudo privileges
#

# Bail out if any of the commands fail
set -e

# Ensure user is passing valid input arguments
if [ "$#" -ne 3 ]; then
  printf "\n"
  echo -e "\t $0 requires 4 args: begin-line-number, end-line-number, input-file"
  printf "\n"
  echo -e "\t e.g. Run script as follows:  $0 3333 3600 /var/log/kern.log"
  printf "\n"
  exit
fi

#
# Collect current time in following format:
# @note: Year, Month, Day, Hour, Minute, Second
# e.g. 2021-12-10-11-27-37
#
timestamp=`printf '%(%Y-%m-%d-%H-%M-%S)T\n'`

#
# Command to extract relevant lines
# e.g. awk "NR < 57890000 { next } { print } NR == 57890010 { exit }" /path-of-input-file
#
echo $timestamp
awk "NR < $1 { next } { print } NR == $2 { exit }" $3

