#!/bin/bash

for ((i=1;i<=${1};i++))
do
	./MOVELOG
	echo "done $i"
done

