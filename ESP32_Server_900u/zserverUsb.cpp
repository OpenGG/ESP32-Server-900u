#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zserverApp.h"
#include "zusb.h"

static void handleUsbOn(AsyncWebServerRequest* request)
{
    zdebug("/admin/usb/on");
    String msg = zusb::enable();
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
    String msg = zusb::disable();
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
