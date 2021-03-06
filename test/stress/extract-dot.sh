#!/bin/bash
# This file is part of fdns project
# Copyright (C) 2019-2021 FDNS Authors
# License GPL v2

# we are coming into this file as root user; by the end we will switch back to the regular $USER

export MALLOC_CHECK_=3
export MALLOC_PERTURB_=$(($RANDOM % 255 + 1))

SERVERS=`fdns --list=all --transport=dot | grep -v https | grep -v zone | grep -v server | awk '{ print $1 }'`

let i=1
for s in $SERVERS
do
	./extract-keepalive.exp $s 127.127.1.$i  | grep  'TESTING\|(keepalive' &
	sleep 1
	let i=i+1
done
sleep 630

