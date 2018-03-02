#!/bin/bash
#
#
#wuyujie


if [[ UID -ne 0 ]]; then
	#statements
	echo "must be root ..."
	sleep 1
	exit 1
fi

if [[ $#  -le 1 ]]; then
	#statements
	echo "need a filename ,must be c file "
	sleep 1
	exit 1
fi
# $2 target $1 src file
gcc -o $2  $1

if [[ $? -ne 0 ]]; then
	#statements
	echo "gcc exec error..."
	sleep 1
	exit 1
fi

echo "build success..."

#dos2unix $0
#
#if [[ $? -ne 0 ]]; then
#	#statements
#	echo "dos2unix format  error..."
#	sleep 1
#	exit 1
#fi







