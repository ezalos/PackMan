#!/bin/bash

# Usage: sh recursive_test.sh binary nb_of_times

yellow='\e[33m'
reset='\e[0m'


if [ "$#" -eq 2 ]; then
	echo $yellow "TESTOR--> Welcome to recursive_test for woodywood_packer.out !" $reset
else
	echo $yellow "TESTOR--> ERROR: $@" $reset
	echo $yellow "TESTOR--> Usage: sh recursive_test.sh binary_to_test nb_of_times" $reset
	exit
fi

print_and_quit()
{
  echo "TESTOR--> There has been a segfault !"
  exit
}


test_ret()
{
	if [ $1 -eq 139 ]; then
		echo $yellow "TESTOR--> Ret is 139, maybe SEGV, EXITING PROGRAM" $reset
		exit
	elif [ $1 -eq 0 ]; then
		echo $yellow "TESTOR--> Normal ret" $reset
	else
		echo $yellow "TESTOR--> Weird, ret_val is unexpected" $reset
		exit
	fi

}

trap print_and_quit SEGV

echo $yellow "TESTOR--> $1 will be launched recursively $2 times" $reset

./woody_woodpacker $1

./woody.out

n=1
while [ $n -le $2 ]; do {

	echo $yellow "TESTOR--> Test following: $n / $2" $reset
	./woody_woodpacker ./woody.out
	ret_val=$?
	echo $yellow "TESTOR--> Return value of woody_woodpacker.out is $ret_val for test: $n / $2" $reset
	test_ret $ret_val
	./woody.out
	ret_val=$?
	echo $yellow "TESTOR--> Return value of woody.out is $ret_val for test: $n / $2" $reset
	test_ret $ret_val
	n=$(( n+1 ))	 # increments $n
	}
done