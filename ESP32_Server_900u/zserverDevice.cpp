#include "ESPAsyncWebServer.h"
#include "zconfig.h"
#include "zdebug.h"
#include "zfs.h"
#include "zserverApp.h"
#include "zsleep.h"

static bool isFormating = false;

static void handleFormat(AsyncWebServerRequest* request)
{
    isFormating = true;

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, "Formatting, wait for it to reboot.");
}

static void handleReset(AsyncWebServerRequest* request)
{
    zconfig::clear();

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleReboot(AsyncWebServerRequest* request)
{
    ESP.restart();

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
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
    String output = "";
    output += "###### Software ######\n";
    output += "SDK version: " + String(ESP.getSdkVersion()) + "\n";
    output += "\n";
    output += "###### Board ######\n";
    output += "MCU: " + mcuType + "\n";
#if defined(USB_PRODUCT)
    output += "Board: " + String(USB_PRODUCT) + "\n";
#endif
    output += "Chip Id: " + String(ESP.getChipModel()) + "\n";
    output += "CPU frequency: " + String(ESP.getCpuFreqMHz()) + "MHz\n";
    output += "Cores: " + String(ESP.getChipCores()) + "\n";
    output += "\n";
    output += "###### Flash chip information ######\n";
    output += "Flash chip Id: " + String(ESP.getFlashChipMode()) + "\n";
    output += "Estimated Flash size: " + formatBytes(ESP.getFlashChipSize()) + "\n";
    output += "Flash frequency: " + String(flashFreq) + " MHz\n";
    output += "Flash write mode: " + String((ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT"
                      : ideMode == FM_DIO                                                   ? "DIO"
                      : ideMode == FM_DOUT                                                  ? "DOUT"
                                                                                            : "UNKNOWN"))
        + "\n";
    output += "\n";
    output += "###### Storage information ######\n";
    output += "Total Size: " + formatBytes(zfs.totalBytes()) + "\n";
    output += "Used Space: " + formatBytes(zfs.usedBytes()) + "\n";
    output += "Free Space: " + formatBytes(zfs.totalBytes() - zfs.usedBytes()) + "\n";
    output += "\n";
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    if (ESP.getPsramSize() > 0) {
        output += "###### PSRam information ######\n";
        output += "Psram Size: " + formatBytes(ESP.getPsramSize()) + "\n";
        output += "Free psram: " + formatBytes(ESP.getFreePsram()) + "\n";
        output += "Max alloc psram: " + formatBytes(ESP.getMaxAllocPsram()) + "\n";
        output += "\n";
    }
#endif
    output += "###### Ram information ######\n";
    output += "Ram size: " + formatBytes(ESP.getHeapSize()) + "\n";
    output += "Free ram: " + formatBytes(ESP.getFreeHeap()) + "\n";
    output += "Max alloc ram: " + formatBytes(ESP.getMaxAllocHeap()) + "\n";
    output += "\n";
    output += "###### Sketch information ######\n";
    output += "Sketch hash: " + ESP.getSketchMD5() + "\n";
    output += "Sketch size: " + formatBytes(ESP.getSketchSize()) + "\n";
    output += "Free space available: " + formatBytes(ESP.getFreeSketchSpace() - ESP.getSketchSize()) + "\n";
    output += "\n";
    output += "###### Config information #####\n";
    output += "wifi_connect: " + zconfig::get("wifi_connect", "") + "\n";
    output += "wifi_ap_ssid: " + zconfig::get("wifi_ap_ssid", "") + "\n";
    output += "wifi_ap_pass: " + zconfig::get("wifi_ap_pass", "") + "\n";
    output += "wifi_ap_ip: " + zconfig::get("wifi_ap_ip", "") + "\n";
    output += "wifi_ap_ch: " + zconfig::get("wifi_ap_ch", "") + "\n";
    output += "wifi_ssid: " + zconfig::get("wifi_ssid", "") + "\n";
    output += "wifi_pass: " + zconfig::get("wifi_pass", "") + "\n";
    output += "wifi_hostname: " + zconfig::get("wifi_hostname", "") + "\n";
    output += "usb_pin: " + zconfig::get("usb_pin", "") + "\n";
    output += "device_sleep: " + zconfig::get("device_sleep", "") + "\n";
    output += "\n";
    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, output);
}

static void handleSleep(AsyncWebServerRequest* request)
{
    zsleep::sleep();

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, "Going to sleep");
}

namespace zroutes {
void device()
{
    zserverApp.on("/admin/device/format", HTTP_POST, handleFormat);

    zserverApp.on("/admin/device/reset", HTTP_POST, handleReset);

    zserverApp.on("/admin/device/reboot", HTTP_POST, handleReboot);

    zserverApp.on("/admin/device/info", HTTP_GET, handleInfo);

    zserverApp.on("/admin/device/sleep", HTTP_POST, handleSleep);
}

void deviceLoop()
{
    if (isFormating) {
        isFormating = false;

        zfs.end();
        zfs.format();
        zfs.begin(true);
        delay(1000);

        ESP.restart();
    }
}
}
