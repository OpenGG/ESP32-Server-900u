#include "zusbConfig.h"

#ifdef Z_USB_MSC

#include "USB.h"
#include "USBMSC.h"
#include "zconfig.h"
#include "zdebug.h"
#include "zfs.h"
#include "zmsg.h"
#include <math.h>

#define Z_DISK_SECTOR_COUNT 8192
#define Z_DISK_SECTOR_SIZE 512
#define Z_USB_BIN_PATH "/usb.bin"

// static uint32_t Z_DISK_SECTOR_SIZE = 512;

static USBMSC msc;

static File file;

static uint32_t maxSectorIndex = 0;

static uint8_t readBuff[Z_DISK_SECTOR_SIZE];

static void closeFile()
{
    maxSectorIndex = 0;
    file.close();
}

static int32_t onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    lba = min(lba, maxSectorIndex);
    uint32_t seekIndex = lba * Z_DISK_SECTOR_SIZE;
    uint32_t sectorSize = Z_DISK_SECTOR_SIZE;
    uint32_t readSize = min(sectorSize, file.size() - seekIndex);

    bool shouldClear = readSize != bufsize;
    if (shouldClear) {
        memset(readBuff, 0, Z_DISK_SECTOR_SIZE);
    }

    file.seek(seekIndex);
    file.read(readBuff, readSize);
    memcpy(buffer, readBuff, bufsize);

    zdebug("usb.bin onRead. seek: ", seekIndex);
    zdebug("usb.bin onRead. read: ", readSize);
    zdebug("usb.bin onRead. buffer: ", bufsize);

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

bool enable(char* msg, size_t n)
{
    if (hasEnabled) {
        zmsg(msg, "Usb already enabled", n);
        return false;
    }

    maxSectorIndex = 0;

    file = zfs.open(Z_USB_BIN_PATH, "r");

    zdebug("zusbMsc::enable()");

    if (!file) {
        zdebug("zusbMsc::enable(): file not found");

        zmsg(msg, "File not exists: " Z_USB_BIN_PATH, n);
        return false;
    }

    int size = file.size();

    zdebug("zusbMsc::enable(): file size ", size);

    if (size == 0) {
        file.close();
        zmsg(msg, "Empty file: " Z_USB_BIN_PATH, n);
        return false;
    }

    maxSectorIndex = ceil((float)size / (float)Z_DISK_SECTOR_SIZE) - 1;

    zdebug("zusbMsc::enable(): maxSectorIndex ", maxSectorIndex);

    msc.vendorID("ESP");
    msc.productID(ESP.getChipModel());
    msc.productRevision("1.0");
    msc.onRead(onRead);
    msc.mediaPresent(true);

    bool success = msc.begin(Z_DISK_SECTOR_COUNT, Z_DISK_SECTOR_SIZE);
    zdebug("DISK_SECTOR_COUNT: ", Z_DISK_SECTOR_COUNT);
    zdebug("Z_DISK_SECTOR_SIZE: ", Z_DISK_SECTOR_SIZE);
    zdebug("usbmsc.begin(): ", success);

    // if (!success) {
    //     return "Failed to call usbmsc.begin()";
    // }

    success = USB.begin();
    zdebug("USB.begin(): ", success);

    // if (!success) {
    //     return "Failed to call USB.begin()";
    // }

    enTime = millis();
    hasEnabled = true;

    return true;
}

bool disable(char* msg, size_t n)
{
    if (!hasEnabled) {
        zmsg(msg, "Usb not enabled", n);
        return false;
    }

    requestDisable = true;

    zdebug("zusbMsc::disable()");

    return true;
}

void loop()
{
    if (hasEnabled) {
        if (requestDisable) {
            // manual disable

            hasEnabled = false;

            doDisable();
        } else if (millis() >= (enTime + Z_USB_TTL)) {
            // auto disable
            hasEnabled = false;
            zdebug("zusbMsc::disable(): auto");

            doDisable();
        }
    }
}
}

#endif