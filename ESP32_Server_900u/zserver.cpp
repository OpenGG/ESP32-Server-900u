#include "ESPAsyncWebServer.h"
#include "zserverApp.h"
#include "zserverDevice.h"
#include "zserverFiles.h"
#include "zserverNotFound.h"
#include "zserverUsb.h"
#include "zserverConnTest.h"
#include "zfs.h"

namespace zserver
{
    void setup()
    {
        // admin apis
        zroutes::device();
        zroutes::files();
        zroutes::usb();
        zroutes::connTest();

        // static server
        zserverApp.serveStatic("/", zfs, "/")
            .setDefaultFile("index.html")
            // cache for 1 year
            .setCacheControl("max-age=31536000");

        // fallback 404
        zroutes::notFound();
        zserverApp.begin();
    }

    void loop()
    {
        zroutes::usbLoop();
    }
}
