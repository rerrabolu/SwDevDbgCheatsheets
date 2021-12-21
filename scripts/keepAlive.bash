#!/bin/bash

#
# Execute while loop until issued ctrl-c
# To keep a shell from exiting, Run the script in background mode
#   e.g. keepAlive.bash &
#
sleepTime='3m'
while :
 do
  echo "Keep shell alive by loop over sleep: sleep-time = $sleepTime"
  sleep $sleepTime
 done
 
