#!/bin/bash

function quit() {
	done=1
}

trap quit SIGINT

done=0

while [ "$done" -eq 0 ]; do
	/opt/vc/bin/raspistill -w 320 -h 240 -o camera.jpg -t 1 -q 50
	mosquitto_pub -t /polarsys-rover/front-camera -f camera.jpg
done
