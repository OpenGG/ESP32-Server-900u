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

if [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX
    esptool=$(ls $HOME/Library/Arduino15/packages/esp32/tools/esptool_py/*/esptool)
    otaBin=$(ls $HOME/Library/Arduino15/packages/esp32/hardware/esp32/*/tools/partitions/boot_app0.bin)
else

    esptool="python $(ls $HOME/.arduino15/packages/esp32/tools/esptool_py/*/esptool.py)"
    otaBin=$(ls $HOME/.arduino15/packages/esp32/hardware/esp32/*/tools/partitions/boot_app0.bin)
fi

cp $fs_bin data.lfs.bin

$esptool --chip "$chip" \
    merge_bin \
    -o $folder.merged.bin \
    --flash_mode dio \
    --flash_size 4MB \
    --flash_freq 80m \
    0x1000 *.ino.bootloader.bin \
    0x8000 *.ino.partitions.bin \
    0xe000 $otaBin \
    0x10000 *.ino.bin \
    0x290000 data.lfs.bin
