#!/usr/bin/bash

#
# Start agent if the list of keys it is
# serving is empty
#
if ! ssh-add -l >/dev/null 2>&1; then
	eval $(ssh-agent)
else
	echo "ssh-agent is already running"
fi

#
# Define the list of ssh keys you want
# to add to ssh-agent
#
# @note: Be sure the following private key
# files exist on the client system
#
sshKeysList=(
	"$HOME/.ssh/rerrabolu_ed25519"
	"$HOME/.ssh/linuxOnZ_ed25519"
	"$HOME/.ssh/lnxGate_ed25519"
	"$HOME/.ssh/ramIbm_ed25519"
)

#
# Iterate through the list of private keys
# and add them to the ssh-agent
#
for sshKey in "${sshKeysList[@]}"; do

	if [[ ! -f "$sshKey" ]]; then
		echo "WARNING: Key file not found: $sshKey"
		continue
	fi

	keyVal=$(ssh-keygen -lf "$sshKey" | cut -d' ' -f 2)

	if ! ssh-add -l | grep -qF "$keyVal"; then
		echo "Adding key: $sshKey"
		ssh-add -t 7200 "$sshKey"
	else
		echo "Not Adding key: $sshKey"
	fi
	echo ""
	echo ""
done


