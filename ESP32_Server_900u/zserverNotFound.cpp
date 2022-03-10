#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zserverApp.h"

static void handleNotFound(AsyncWebServerRequest* request)
{
    zdebug("404 request");
    zdebug("version: " + String(request->version()));
    zdebug("method: " + String(request->method()));
    zdebug("url: " + String(request->url()));
    zdebug("host: " + String(request->host()));
    zdebug("contentType: " + String(request->contentType()));
    zdebug("contentLength: " + String(request->contentLength()));
    zdebug("multipart: " + String(request->multipart()));

    request->send(404, Z_MIME_PLAIN_TEXT, "Not found");
}

namespace zroutes {
void notFound()
{
    zserverApp.onNotFound(handleNotFound);
}
}
