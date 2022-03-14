#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zserverApp.h"

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#include "zusbMsc.h"
#define zusbImpl zusbMsc
#else
#include "zusb.h"
#define zusbImpl zusb
#endif

static void handleUsbOn(AsyncWebServerRequest* request)
{
    zdebug("/admin/usb/on");
    String msg = zusbImpl::enable();
    zdebug("/admin/usb/on: " + msg);

    if (msg.length() != 0) {
        request->send(503, Z_MIME_PLAIN_TEXT, msg);
        return;
    }

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleUsbOff(AsyncWebServerRequest* request)
{
    zdebug("/admin/usb/off");
    String msg = zusbImpl::disable();
    zdebug("/admin/usb/off: " + msg);

    if (msg.length() != 0) {
        request->send(503, Z_MIME_PLAIN_TEXT, msg);
        return;
    }

    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

namespace zroutes {
void usb()
{
    zusbImpl::setup();
    zserverApp.on(
        "/admin/usb/on", HTTP_POST, handleUsbOn);
    zserverApp.on(
        "/admin/usb/off", HTTP_POST, handleUsbOff);
}

void usbLoop()
{
    zusbImpl::loop();
}
}
