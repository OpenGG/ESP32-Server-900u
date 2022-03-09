#!/usr/bin/env bash

set -e

npm run build

dir="docs"

index=index.html

cd $dir

echo "" > $index

for d in *
do
    if [ -d "$d" ]
    then
        echo "<a style=\"font-size:18px\" href=./$d>$d</a>" >> $index
    fi
done

echo -e "$(cat $index|sort -u -rV)" > $index
