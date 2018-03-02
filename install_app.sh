#!/bin/bash
#
#
#
#wuyujie

APP="./targets"

if [[ UID -ne 0 ]]; then
	#statements
	echo "must be root..."
	sleep 1
	exit 1
fi

if [[ ! -x ${APP} ]]; then
	#statements
	echo "no application ,please check it "
	sleep 1
	exit 1
fi

mkdir  ./APPLICATION/  -p

for i in $(ls  *.cf)
do 
   echo "${i} is copying"
   cp ${i}  ./APPLICATION/
done

cp start.sh  ./APPLICATION

cp ${APP}   ./APPLICATION/

echo "install success..."

sleep 1


