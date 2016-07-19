#!/bin/sh
nc -l 9999 | mplayer -fps 20 -demuxer h264es - 
