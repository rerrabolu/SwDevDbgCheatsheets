#!/bin/bash

termName=$1

#
# Define a bash function to set title
#
function setTermTitle() {
  TITLE=$1
  if [[ -z "$ORIG" ]]; then
    ORIG=$PS1
  fi
  TITLE="\[\e]2;$*\a\]"
  PS1=${ORIG}${TITLE}
}

# Use the function to name the terminal
setTermTitle $termName


