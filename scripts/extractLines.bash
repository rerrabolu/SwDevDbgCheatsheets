#!/bin/bash

# Ensure user is passing valid values
if [ "$#" -ne 2 ]; then
  echo "$0 requires 2 args: inputLogFile followed by outputLogFile"
  echo "Run script as follows:  $0 /home/user/foo.txt  /home/user/boo.txt"
  exit
fi

#
# Capture current working directory
#
currDir=${PWD}

# List of tokens to search for
tokenList=(
      "foo"
      "boo"
      "doo"
      "moo"
      );

# Set this to the file you want to parse
logFile="$1"
outFile="$2"
touch $logFile
touch $outFile

#
# Read the input file, line by line. For each line
# check if it has at least one token from the list
# of tokens. If present then write the line into
# the output file
#
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
