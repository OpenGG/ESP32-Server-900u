#include <Arduino.h>
#include <math.h>
#include "zconfig.h"
#include "USB.h"
#include "USBMSC.h"
#include "zfs.h"
#include "zdebug.h"

#define MAX_USB_TTL 15000
#define DISK_SECTOR_SIZE 512
#define USB_BIN_PATH "/usb.bin"

// static uint32_t DISK_SECTOR_SIZE = 512;

static USBMSC dev;

static File file;

static uint32_t maxSectorIndex = 0;

static uint8_t readBuff[DISK_SECTOR_SIZE] = {0};

static void closeFile()
{
    maxSectorIndex = 0;
    file.close();
}

static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
    lba = min(lba, maxSectorIndex);
    uint32_t seekIndex = lba * DISK_SECTOR_SIZE;
    uint32_t sectorSize = DISK_SECTOR_SIZE;
    uint32_t readSize = min(sectorSize, file.size() - seekIndex);

    if (readSize != bufsize)
    {
        memset(readBuff, 0, DISK_SECTOR_SIZE);
    }

    file.seek(seekIndex);
    file.read(readBuff, readSize);
    memcpy(buffer, readBuff, bufsize);

    return bufsize;
}

static long enTime = 0;
static bool hasEnabled = false;

namespace zusbCustom
{
    void setup()
    {
    }

    String enable()
    {
        maxSectorIndex = 0;

        file = zfs.open(USB_BIN_PATH, "r");
        if (!file)
        {
            return String("File not exists: ") + String(USB_BIN_PATH);
        }

        int size = file.size();
        if (size == 0)
        {
            file.close();
            return String("Empty file: ") + String(USB_BIN_PATH);
        }

        maxSectorIndex = ceil(size / DISK_SECTOR_SIZE) - 1;

        dev.vendorID("PS4");
        dev.productID("ESP32 Server");
        dev.productRevision("1.0");
        dev.onRead(onRead);
        dev.mediaPresent(true);

        int sectorCount = zconfig::get("usb_sc", "8192").toInt();

        zdebug("sectorCount: " + String(sectorCount));

        dev.begin(sectorCount, DISK_SECTOR_SIZE);
        USB.begin();
        enTime = millis();
        hasEnabled = true;

        return "";
    }

    String disable()
    {
        enTime = 0;
        hasEnabled = false;
        closeFile();
        dev.end();
        // USB.end();
        ESP.restart();

        return "";
    }

    void loop()
    {
        if (hasEnabled && millis() >= (enTime + MAX_USB_TTL))
        {
            zusbCustom::disable();
            closeFile();
        }
    }
}
