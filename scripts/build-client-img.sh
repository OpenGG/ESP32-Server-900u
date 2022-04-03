#!/usr/bin/env bash

set -e

if [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX
    mklittlefs=$(ls $HOME/Library/Arduino15/packages/esp32/tools/mklittlefs/*/mklittlefs)
else

    mklittlefs=$(ls $HOME/.arduino15/packages/esp32/tools/mklittlefs/*/mklittlefs)
fi

# fs size of 0x170000
$mklittlefs -c "./data" -b 4096 -p 1024 -s 1507328 lfs.out
