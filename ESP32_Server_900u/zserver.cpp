#include "ESPAsyncWebServer.h"
#include "zserverApp.h"
#include "zserverCacheManifest.h"
#include "zserverConnTest.h"
#include "zserverDevice.h"
#include "zserverFiles.h"
#include "zserverFs.h"
#include "zserverNotFound.h"
#include "zserverPsMock.h"
#include "zserverUsb.h"

namespace zserver {
void setup()
{
    // admin apis
    zroutes::device();
    zroutes::files();
    zroutes::usb();
    zroutes::connTest();
    zroutes::cacheManifest();
    zroutes::psMock();

    // static server
    zroutes::fs();

    // fallback 404
    zroutes::notFound();
    zserverApp.begin();
}

void loop()
{
    zroutes::usbLoop();
    zroutes::deviceLoop();
}
}
