#include "ESPAsyncWebServer.h"
#include "zserverApp.h"

#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
#include "zusbCustom.h"
#define zusbImpl zusbCustom
#else
#include "zusb.h"
#define zusbImpl zusb
#endif

static void handleUsbOn(AsyncWebServerRequest *request)
{
    String msg = zusbImpl::enable();
    if (msg.length() == 0) {
        request->send(503, Z_MIME_PLAIN_TEXT, msg);
        return;
    }

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}


static void handleUsbOff(AsyncWebServerRequest *request)
{
    String msg = zusbImpl::disable();
    if (msg.length() == 0) {
        request->send(503, Z_MIME_PLAIN_TEXT, msg);
        return;
    }

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

namespace zroutes
{
    void usb()
    {
        zusbImpl::setup();
        zserverApp.on(
            "/admin/usb/on", HTTP_POST, [](AsyncWebServerRequest *request)
            { handleUsbOn(request); });
        zserverApp.on(
            "/admin/usb/off", HTTP_POST, [](AsyncWebServerRequest *request)
            { handleUsbOff(request); });
    }

    void usbLoop() {
        zusbImpl::loop();
    }
}
