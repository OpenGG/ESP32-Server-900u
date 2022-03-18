#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zfs.h"
#include "zserverApp.h"

static bool filterOnBin(AsyncWebServerRequest* request)
{
    return request->url().endsWith(".bin");
}

namespace zroutes {
void fs()
{
    zserverApp.serveStatic("/", zfs, "/")
        .setCacheControl("max-age=31536000")
        // cache for 1 year
        .setFilter(filterOnBin);

    zserverApp.serveStatic("/", zfs, "/")
        .setDefaultFile("index.html");
}
}
