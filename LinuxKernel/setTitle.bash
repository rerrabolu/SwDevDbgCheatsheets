#!/bin/bash

#
# Update gnome terminal title
# Value of PS1 is set in .bashrc
# function setTitle() { OLD_PS1=${PS1}; PS1="${PS1/\\u@\\h: \\w/$@}"; }
# function unsetTitle() { PS1="${OLD_PS1}"; }
#

 
#
# Define a bash function to set title
#
function setTitle() {
  TITLE=$1
  if [[ -z "$ORIG" ]]; then
    ORIG=$PS1
  fi
  TITLE="\[\e]2;$*\a\]"
  PS1=${ORIG}${TITLE}
}

