#!/usr/bin/bash


manFile=$1
htmlFile=$2

# echo "File of Man pages is: $1"
# echo "File of Html page is: $2"

# Convert Man page content into a HTML file so
# that one can read them in a more convenient format
#   groff -Thtml -man <path-of-man-page>  <path-of-html-page>
#   groff -Thtml -man lszdev.  $HOME/ZManPages/lszdev.html
#
groff -Thtml -man ${1} > ${2} 2> /dev/null


