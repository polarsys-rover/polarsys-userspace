#!/bin/bash

function quit() {
	done=1
}

function usage() {
	echo "Usage: ./camera-publish-simulated.sh DIR PERIOD"
	echo
	echo "  DIR should be a directory containing .jpg images."
	echo "  PERIOD is the time in seconds to wait between each image.  If the"
	echo "    sleep program in use is GNU sleep, this value can be a fraction"
	echo "    of a second (e.g. 0.5)."
}

trap quit SIGINT

done=0

if [ "$#" -ne "2" ]; then
	usage
	exit 1
fi

directory=$1
period=$2
topic="/polarsys-rover/front-camera"

while [ "$done" -eq 0 ]; do
	for file in "$directory/"*".jpg"; do
		mosquitto_pub -t "$topic" -f "$file"
		sleep "$period"

		if [ "$done" -ne 0 ]; then
			break
		fi
	done
done
