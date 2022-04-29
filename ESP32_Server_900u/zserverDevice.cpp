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

static char* formatBytes(char* buff, size_t size, size_t bytes)
{
    float num = 0;

    if (bytes < 1024) {
        num = bytes;
    } else if (bytes < (1024 * 1024)) {
        num = bytes / 1024.0;
    } else if (bytes < (1024 * 1024 * 1024)) {
        num = bytes / 1024.0 / 1024.0;
    } else {
        num = bytes / 1024.0 / 1024.0 / 1024.0;
    }

    // memset(buff, 0, size);

    snprintf(buff, size - 3, "%f", num);

    if (bytes < 1024) {
        strcat(buff, " B");
    } else if (bytes < (1024 * 1024)) {
        strcat(buff, " KB");
    } else if (bytes < (1024 * 1024 * 1024)) {
        strcat(buff, " MB");
    } else {
        strcat(buff, " GB");
    }

    return buff;
}

static void handleInfo(AsyncWebServerRequest* request)
{
    char buffer[10];
    float flashFreq = (float)ESP.getFlashChipSpeed() / 1000.0 / 1000.0;
    FlashMode_t ideMode = ESP.getFlashChipMode();
    // String mcuType = CONFIG_IDF_TARGET;
    // mcuType.toUpperCase();
    String output = "";
    output += "###### Software ######\n";
    output += "SDK version: ";
    output += ESP.getSdkVersion();
    output += "\n";
    output += "\n";
    output += "###### Board ######\n";
    output += "MCU: ";
    output += CONFIG_IDF_TARGET;
    output += "\n";
#if defined(USB_PRODUCT)
    output += "Board: ";
    output += USB_PRODUCT;
    output += "\n";
#endif
    output += "Chip Id: ";
    output += ESP.getChipModel();
    output += "\n";
    output += "CPU frequency: ";
    output += ESP.getCpuFreqMHz();
    output += " MHz\n";
    output += "Cores: ";
    output += ESP.getChipCores();
    output += "\n";
    output += "\n";
    output += "###### Flash chip information ######\n";
    output += "Flash chip Id: ";
    output += ESP.getFlashChipMode();
    output += "\n";
    output += "Estimated Flash size: ";
    output += formatBytes(buffer, sizeof(buffer), ESP.getFlashChipSize());
    output += "\n";
    output += "Flash frequency: ";
    output += flashFreq;
    output += " MHz\n";
    output += "Flash write mode: ";
    output += ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT"
        : ideMode == FM_DIO                                  ? "DIO"
        : ideMode == FM_DOUT                                 ? "DOUT"
                                                             : "UNKNOWN";
    output += "\n";
    output += "\n";
    output += "###### Storage information ######\n";
    output += "Total Size: ";
    output += formatBytes(buffer, sizeof(buffer), zfs.totalBytes());
    output += "\n";
    output += "Used Space: ";
    output += formatBytes(buffer, sizeof(buffer), zfs.usedBytes());
    output += "\n";
    output += "Free Space: ";
    output += formatBytes(buffer, sizeof(buffer), zfs.totalBytes() - zfs.usedBytes());
    output += "\n";
    output += "\n";
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    if (ESP.getPsramSize() > 0) {
        output += "###### PSRam information ######\n";
        output += "Psram Size: ";
        output += formatBytes(buffer, sizeof(buffer), ESP.getPsramSize());
        output += "\n";

        output += "Free psram: ";
        output += formatBytes(buffer, sizeof(buffer), ESP.getFreePsram());
        output += "\n";

        output += "Max alloc psram: ";
        output += formatBytes(buffer, sizeof(buffer), ESP.getMaxAllocPsram());
        output += "\n";
        output += "\n";
    }
#endif
    output += "###### Ram information ######\n";
    output += "Ram size: ";
    output += formatBytes(buffer, sizeof(buffer), ESP.getHeapSize());
    output += "\n";

    output += "Free ram: ";
    output += formatBytes(buffer, sizeof(buffer), ESP.getFreeHeap());
    output += "\n";

    output += "Max alloc ram: ";
    output += formatBytes(buffer, sizeof(buffer), ESP.getMaxAllocHeap());
    output += "\n";

    output += "\n";

    output += "###### Sketch information ######\n";
    output += "Sketch hash: ";
    output += ESP.getSketchMD5();
    output += "\n";

    output += "Sketch size: ";
    output += formatBytes(buffer, sizeof(buffer), ESP.getSketchSize());
    output += "\n";

    output += "Free space available: ";
    output += formatBytes(buffer, sizeof(buffer), ESP.getFreeSketchSpace() - ESP.getSketchSize());
    output += "\n";

    output += "\n";

    output += "###### Config information ######\n";
    output += "wifi_connect: ";
    output += zconfig::getInt("wifi_connect", -1);
    output += "\n";

    output += "wifi_ap_ssid: ";
    output += zconfig::get("wifi_ap_ssid", "");
    output += "\n";

    output += "wifi_ap_pass: ";
    output += zconfig::get("wifi_ap_pass", "");
    output += "\n";

    output += "wifi_ap_ip: ";
    output += zconfig::get("wifi_ap_ip", "");
    output += "\n";

    output += "wifi_ap_ch: ";
    output += zconfig::getInt("wifi_ap_ch", -1);
    output += "\n";

    output += "wifi_ssid: ";
    output += zconfig::get("wifi_ssid", "");
    output += "\n";

    output += "wifi_pass: ";
    output += zconfig::get("wifi_pass", "");
    output += "\n";

    output += "wifi_hostname: ";
    output += zconfig::get("wifi_hostname", "");
    output += "\n";

    output += "usb_pin: ";
    output += zconfig::getInt("usb_pin", -1);
    output += "\n";

    output += "device_sleep: ";
    output += zconfig::get("device_sleep", "");
    output += "\n";

    output += "\n";

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, output);
}

static void handleSleep(AsyncWebServerRequest* request)
{
    zsleep::sleep();

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, "Going to sleep");
}

static void handleSetSleep(AsyncWebServerRequest* request)
{
    int secs = 0;

    AsyncWebParameter* p = request->getParam("secs");

    if (p != NULL) {
        secs = p->value().toInt();
    }

    if (secs > 0 && secs < 3600) {
        zsleep::setSleep(secs);
        request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
        return;
    }

    request->send(Z_STATUS_CLIENT_ERROR, Z_MIME_PLAIN_TEXT, "Invalid input");
}

namespace zroutes {
void device()
{
    zserverApp.on("/admin/device/format", HTTP_POST, handleFormat);

    zserverApp.on("/admin/device/reset", HTTP_POST, handleReset);

    zserverApp.on("/admin/device/reboot", HTTP_POST, handleReboot);

    zserverApp.on("/admin/device/info", HTTP_GET, handleInfo);

    zserverApp.on("/admin/device/sleep/now", HTTP_POST, handleSleep);

    zserverApp.on("/admin/device/sleep/set", HTTP_POST, handleSleep);
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
