#!/usr/bin/env bash

for file in ./*.merged.bin
do
    if [ -e "$file" ]
    then
        base=$(basename $file .bin)
        zip $base.zip $file
        7z a $base.7z $file
    fi
done
