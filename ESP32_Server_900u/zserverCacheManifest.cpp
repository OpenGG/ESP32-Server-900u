#include "ESPAsyncWebServer.h"
#include "zfs.h"
#include "zserverApp.h"

#define Z_CACHE_MANIFEST "/cache.manifest"
#define Z_MINE_CACHE_MANIFEST "text/cache-manifest"

static void handleCacheManifest(AsyncWebServerRequest* request)
{
    if (!zfs.exists(Z_CACHE_MANIFEST)) {
        request->send(200, Z_MIME_PLAIN_TEXT, "Not found");
        return;
    }

    AsyncWebServerResponse* response = request->beginResponse(zfs, Z_CACHE_MANIFEST, Z_MINE_CACHE_MANIFEST);

    response->addHeader("Cache-Control", "max-age=31536000");

    request->send(response);
}

namespace zroutes {
void cacheManifest()
{
    zserverApp.on(Z_CACHE_MANIFEST, HTTP_GET, handleCacheManifest);
}
}
