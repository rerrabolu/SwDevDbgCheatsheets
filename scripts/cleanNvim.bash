#!/usr/bin/bash

# List of resources to clear / delete
rsrcList=(
    "$HOME/.local/share/nvim/lazy"
    "$HOME/.local/state/nvim/lazy"
    "$HOME/.config/nvim/lazy-lock.json"
)


# Run delete command on each element
for elem in "${rsrcList[@]}"; do
  echo "Removing resource: $elem"
  rm -rf $elem
done

