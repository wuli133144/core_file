


DIR=./APPLICATION


if [[ UID -ne 0 ]]; then
	#statements
	echo "must be root..."
	sleep 1
	exit 1
fi
	
if [[ ! -d  ${DIR}  ]]; then
	#statements
	echo "can't find this dir,please create it ,then run this shell"
	sleep 1
	exit 1
fi


#check all files ok
#step 1
#check config files


cd  ${DIR}

#CONFIG=$(find ./   -type f  -name  "my.cf"  -exec  ls {} \; )
if [[ ! -e "./my.cf" ]]; then
	#statements
	echo "short of config ,please check it"
	sleep 1
	exit 1
fi

if [[ ! -x "./targets" ]]; then
	#statements
	echo "short of execution file"
	sleep 1
    exit 1
fi

./targets  &> /dev/null 

if [[ $? -ne 0 ]]; then
	#statements
	echo "run error ...."
	sleep 1
	exit 1
fi

echo "running ......"
