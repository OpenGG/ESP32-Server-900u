#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zserverApp.h"
#include "zusb.h"

static void handleUsbOn(AsyncWebServerRequest* request)
{
    zdebug("/admin/usb/on");
    char msg[128] = { 0 };
    bool success = zusb::enable(msg, 128);
    zdebug("/admin/usb/on: ", msg);

    if (!success) {
        request->send(Z_STATUS_SERVER_ERROR, Z_MIME_PLAIN_TEXT, msg);
        return;
    }

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

static void handleUsbOff(AsyncWebServerRequest* request)
{
    zdebug("/admin/usb/off");
    char msg[128] = { 0 };
    bool success = zusb::disable(msg, 128);
    zdebug("/admin/usb/off: ", msg);

    if (!success) {
        request->send(Z_STATUS_SERVER_ERROR, Z_MIME_PLAIN_TEXT, msg);
        return;
    }

    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

namespace zroutes {
void usb()
{
    zusb::setup();
    zserverApp.on(
        "/admin/usb/on", HTTP_POST, handleUsbOn);
    zserverApp.on(
        "/admin/usb/off", HTTP_POST, handleUsbOff);
}

void usbLoop()
{
    zusb::loop();
}
}
