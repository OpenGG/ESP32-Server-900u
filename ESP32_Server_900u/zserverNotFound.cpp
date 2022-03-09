#include "ESPAsyncWebServer.h"
#include "zserverApp.h"

static void handleNotFound(AsyncWebServerRequest *request)
{
    request->send(404, Z_MIME_PLAIN_TEXT, "Not found");
}

namespace zroutes
{
    void notFound()
    {
        zserverApp.onNotFound(handleNotFound);
    }
}
