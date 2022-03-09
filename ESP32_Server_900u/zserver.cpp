#include "ESPAsyncWebServer.h"
#include "zserverApp.h"
#include "zserverDevice.h"
#include "zserverFiles.h"
#include "zserverNotFound.h"
#include "zserverUsb.h"
#include "zfs.h"

namespace zserver
{
    void setup()
    {
        zroutes::device();
        zroutes::files();

        zserverApp.serveStatic("/", zfs, "/")
            .setDefaultFile("index.html")
            // cache for 1 year
            .setCacheControl("max-age=31536000");

        zroutes::notFound();
        zroutes::usb();
        zserverApp.begin();
    }

    void loop()
    {
        zroutes::usbLoop();
    }
}
