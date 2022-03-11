#!/usr/bin/env bash

# ~/Library/Arduino15/packages/esp32/tools/esptool_py/3.1.0/esptool -p /dev/cu.usbserial-14230 -b 115200 --before default_reset --after hard_reset --chip ESP32-S2 write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 ESP32_Server_900u.ino.bootloader.bin 0x8000 ESP32_Server_900u.ino.partitions.bin 0x10000 ESP32_Server_900u.ino.bin

# ~/Library/Arduino15/packages/esp32/tools/esptool_py/3.1.0/esptool -p /dev/cu.usbmodem01 -b 115200 --before default_reset --after hard_reset --chip ESP32-S2 read_flash 0 0x400000 out.bin

# upload files

set -xe

cd ./data

for filename in ./*; do
    curl -f -F "file=@${filename}" http://10.1.1.1/admin/files/upload
done

# curl "file=@"