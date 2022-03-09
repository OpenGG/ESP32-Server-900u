#include "ESPAsyncWebServer.h"
#include "zfs.h"
#include "zconfig.h"
#include "zserverApp.h"

static void handleFormat(AsyncWebServerRequest *request)
{
    zfs.end();
    zfs.format();
    zfs.begin(true);

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleReset(AsyncWebServerRequest *request)
{
    zconfig::clear();

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleReboot(AsyncWebServerRequest *request)
{
    ESP.restart();

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

namespace zroutes
{
    void device()
    {
        zserverApp.on("/admin/device/format", HTTP_POST, [](AsyncWebServerRequest *request)
                  { handleFormat(request); });

        zserverApp.on("/admin/device/reset", HTTP_POST, [](AsyncWebServerRequest *request)
                  { handleReset(request); });

        zserverApp.on("/admin/device/reboot", HTTP_POST, [](AsyncWebServerRequest *request)
                  { handleReboot(request); });
    }
}
