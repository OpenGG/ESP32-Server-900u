#include "ESPAsyncWebServer.h"
#include "zconfig.h"
#include "zfs.h"
#include "zserverApp.h"

static void handleFormat(AsyncWebServerRequest* request)
{
    zfs.end();
    zfs.format();
    zfs.begin(true);

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleReset(AsyncWebServerRequest* request)
{
    zconfig::clear();

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleReboot(AsyncWebServerRequest* request)
{
    ESP.restart();

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static String formatBytes(size_t bytes)
{
    if (bytes < 1024) {
        return String(bytes) + " B";
    } else if (bytes < (1024 * 1024)) {
        return String(bytes / 1024.0) + " KB";
    } else if (bytes < (1024 * 1024 * 1024)) {
        return String(bytes / 1024.0 / 1024.0) + " MB";
    } else {
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
    }
}

static void handleInfo(AsyncWebServerRequest* request)
{
    float flashFreq = (float)ESP.getFlashChipSpeed() / 1000.0 / 1000.0;
    FlashMode_t ideMode = ESP.getFlashChipMode();
    String mcuType = CONFIG_IDF_TARGET;
    mcuType.toUpperCase();
    String output = "<!DOCTYPE html>";
    output += "<hr>###### Software ######<br><br>";
    output += "SDK version: " + String(ESP.getSdkVersion()) + "<br><hr>";
    output += "###### Board ######<br><br>";
    output += "MCU: " + mcuType + "<br>";
#if defined(USB_PRODUCT)
    output += "Board: " + String(USB_PRODUCT) + "<br>";
#endif
    output += "Chip Id: " + String(ESP.getChipModel()) + "<br>";
    output += "CPU frequency: " + String(ESP.getCpuFreqMHz()) + "MHz<br>";
    output += "Cores: " + String(ESP.getChipCores()) + "<br><hr>";
    output += "###### Flash chip information ######<br><br>";
    output += "Flash chip Id: " + String(ESP.getFlashChipMode()) + "<br>";
    output += "Estimated Flash size: " + formatBytes(ESP.getFlashChipSize()) + "<br>";
    output += "Flash frequency: " + String(flashFreq) + " MHz<br>";
    output += "Flash write mode: " + String((ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT"
                      : ideMode == FM_DIO                                                   ? "DIO"
                      : ideMode == FM_DOUT                                                  ? "DOUT"
                                                                                            : "UNKNOWN"))
        + "<br><hr>";
    output += "###### Storage information ######<br><br>";
    output += "Total Size: " + formatBytes(zfs.totalBytes()) + "<br>";
    output += "Used Space: " + formatBytes(zfs.usedBytes()) + "<br>";
    output += "Free Space: " + formatBytes(zfs.totalBytes() - zfs.usedBytes()) + "<br><hr>";
#if defined(CONFIG_IDF_TARGET_ESP32S2) | defined(CONFIG_IDF_TARGET_ESP32S3)
    if (ESP.getPsramSize() > 0) {
        output += "###### PSRam information ######<br><br>";
        output += "Psram Size: " + formatBytes(ESP.getPsramSize()) + "<br>";
        output += "Free psram: " + formatBytes(ESP.getFreePsram()) + "<br>";
        output += "Max alloc psram: " + formatBytes(ESP.getMaxAllocPsram()) + "<br><hr>";
    }
#endif
    output += "###### Ram information ######<br><br>";
    output += "Ram size: " + formatBytes(ESP.getHeapSize()) + "<br>";
    output += "Free ram: " + formatBytes(ESP.getFreeHeap()) + "<br>";
    output += "Max alloc ram: " + formatBytes(ESP.getMaxAllocHeap()) + "<br><hr>";
    output += "###### Sketch information ######<br><br>";
    output += "Sketch hash: " + ESP.getSketchMD5() + "<br>";
    output += "Sketch size: " + formatBytes(ESP.getSketchSize()) + "<br>";
    output += "Free space available: " + formatBytes(ESP.getFreeSketchSpace() - ESP.getSketchSize()) + "<br><hr>";
    output += "</html>";
    request->send(200, "text/html", output);
}

namespace zroutes {
void device()
{
    zserverApp.on("/admin/device/format", HTTP_POST, handleFormat);

    zserverApp.on("/admin/device/reset", HTTP_POST, handleReset);

    zserverApp.on("/admin/device/reboot", HTTP_POST, handleReboot);

    zserverApp.on("/admin/device/info", HTTP_GET, handleInfo);
}
}
