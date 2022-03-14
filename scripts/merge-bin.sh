#!/usr/bin/env bash

set -e

get_abs_filename() {
  # $1 : relative filename
  echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
}

fs_bin=$(get_abs_filename lfs.out)

if [[ ! -f $fs_bin ]]
then
    echo "fs.out not exists:" $fs_bin
    exit 1
fi

cd ESP32_Server_900u/build/*/

folder="$(basename $(pwd))"

chip="esp32"

if [[ "$folder" == *"esp32s2"* ]]; then
  chip="esp32s2"
elif [[ "$folder" == *"esp32s3"* ]]; then
  chip="esp32s3"
fi

cp $fs_bin data.lfs.bin

esptool.py --chip "$chip" \
    merge_bin \
    -o merged.bin \
    --flash_mode qio \
    --flash_size 4MB \
    0x1000 *.ino.bootloader.bin \
    0x8000 *.ino.partitions.bin \
    0x10000 *.ino.bin \
    0x290000 data.lfs.bin
