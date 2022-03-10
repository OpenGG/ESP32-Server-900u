#include "USB.h"
#include "USBMSC.h"
#include "zconfig.h"
#include "zdebug.h"
#include "zfs.h"
#include <Arduino.h>
#include <math.h>

#define MAX_USB_TTL 15000
#define DISK_SECTOR_COUNT 8192
#define DISK_SECTOR_SIZE 512
#define USB_BIN_PATH "/usb.bin"

// static uint32_t DISK_SECTOR_SIZE = 512;

static USBMSC msc;

static File file;

static uint32_t maxSectorIndex = 0;

static uint8_t readBuff[DISK_SECTOR_SIZE] = { 0 };

static void closeFile()
{
    maxSectorIndex = 0;
    file.close();
}

static int32_t onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    lba = min(lba, maxSectorIndex);
    uint32_t seekIndex = lba * DISK_SECTOR_SIZE;
    uint32_t sectorSize = DISK_SECTOR_SIZE;
    uint32_t readSize = min(sectorSize, file.size() - seekIndex);

    bool shouldClear = readSize != bufsize;
    if (shouldClear) {
        memset(readBuff, 0, DISK_SECTOR_SIZE);
    }

    file.seek(seekIndex);
    file.read(readBuff, readSize);
    memcpy(buffer, readBuff, bufsize);

    zdebug("usb.bin onRead. seek: " + String(seekIndex));
    zdebug("usb.bin onRead. read: " + String(readSize));
    zdebug("usb.bin onRead. buffer: " + String(bufsize));

    return bufsize;
}

static long enTime = 0;
static bool hasEnabled = false;
static bool requestDisable = false;

static void doDisable()
{
    closeFile();
    msc.end();
    // USB.end();
    ESP.restart();
}

namespace zusbMsc {
void setup()
{
}

String enable()
{
    if (hasEnabled) {
        return "Usb already enabled";
    }

    maxSectorIndex = 0;

    file = zfs.open(USB_BIN_PATH, "r");

    zdebug("zusbMsc::enable()");

    if (!file) {
        zdebug("zusbMsc::enable(): file not found");
        return String("File not exists: ") + String(USB_BIN_PATH);
    }

    int size = file.size();

    zdebug("zusbMsc::enable(): file size " + String(size));

    if (size == 0) {
        file.close();
        return String("Empty file: ") + String(USB_BIN_PATH);
    }

    maxSectorIndex = ceil((float)size / (float)DISK_SECTOR_SIZE) - 1;

    zdebug("zusbMsc::enable(): maxSectorIndex " + String(maxSectorIndex));

    msc.vendorID("ESP");
    msc.productID(ESP.getChipModel());
    msc.productRevision("1.0");
    msc.onRead(onRead);
    msc.mediaPresent(true);

    bool success = msc.begin(DISK_SECTOR_COUNT, DISK_SECTOR_SIZE);
    zdebug("DISK_SECTOR_COUNT: " + String(DISK_SECTOR_COUNT));
    zdebug("DISK_SECTOR_SIZE: " + String(DISK_SECTOR_SIZE));
    zdebug("usbmsc.begin(): " + String(success));

    // if (!success) {
    //     return "Failed to call usbmsc.begin()";
    // }

    success = USB.begin();
    zdebug("USB.begin(): " + String(success));

    // if (!success) {
    //     return "Failed to call USB.begin()";
    // }

    enTime = millis();
    hasEnabled = true;

    return "";
}

String disable()
{
    if (!hasEnabled) {
        return "Usb not enabled";
    }

    requestDisable = true;

    zdebug("zusbMsc::disable()");

    return "";
}

void loop()
{
    if (hasEnabled) {
        if (requestDisable) {
            // manual disable

            hasEnabled = false;

            doDisable();
        } else if (millis() >= (enTime + MAX_USB_TTL)) {
            // auto disable
            hasEnabled = false;
            zdebug("zusbMsc::disable(): auto");

            doDisable();
        }
    }
}
}
