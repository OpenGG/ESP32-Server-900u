# build image
# ~/Library/Arduino15/packages/esp32/tools/esptool_py/3.1.0/esptool --chip ESP32-S2 merge_bin -o merged-flash.bin --flash_mode dio --flash_size 4MB 0x1000 ESP32_Server_900u.ino.bootloader.bin 0x8000 ESP32_Server_900u.ino.partitions.bin 0x10000 ESP32_Server_900u.ino.bin

# ~/Library/Arduino15/packages/esp32/tools/esptool_py/3.1.0/esptool --port /dev/cu.usbserial-14230 write_flash --flash_mode dio --flash_size 4MB 0x0 merged-flash.bin


# ~/Library/Arduino15/packages/esp32/tools/esptool_py/3.1.0/esptool --port /dev/cu.usbserial-14230 write_flash --flash_mode dio --flash_size 4MB 0x10000 ESP32_Server_900u.ino.bin

# upload files

set -xe

cd ./data

for filename in ./*; do
    curl -f -F "file=@${filename}" http://10.1.1.1/admin/files/upload
done

# curl "file=@"