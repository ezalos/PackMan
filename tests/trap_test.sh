#!/bin/bash

ctrlc_count=0

no_ctrlc()
{
    ctrlc_count = $(( ctrlc_count+1 ))
    echo "ctrlc_count: $ctrlc_count"
    if [ $ctrlc_count -eq 1 ]; then
        echo "Stop that."
    elif [ $ctrlc_count -eq 2 ]; then
        echo "Once more and I quit."
    else
        echo "That's it.  I quit."
        exit
    fi
}

trap no_ctrlc INT

while true
do
    echo Sleeping
    sleep 10
done