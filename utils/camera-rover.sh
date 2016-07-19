#!/bin/sh
raspivid -t 0 -w 640 -h 480 -fps 20 -o - | nc 192.168.0.101 9999
