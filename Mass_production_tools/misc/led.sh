#!/bin/bash
status=$1
if [ "$status" = "ok" ]
then
    echo "led has test ok"
elif [ "$status" = "err" ]
then 
    echo "led has test err"
fi
