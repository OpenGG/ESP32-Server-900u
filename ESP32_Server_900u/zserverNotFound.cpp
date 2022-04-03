#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zserverApp.h"

static void handleNotFound(AsyncWebServerRequest* request)
{
    zdebug("404 request");
    zdebug("version: ", request->version());
    zdebug("method: ", request->method());
    zdebug("url: ", request->url());
    zdebug("host: ", request->host());
    zdebug("contentType: ", request->contentType());
    zdebug("contentLength: ", request->contentLength());
    zdebug("multipart: ", request->multipart());

    request->send(Z_STATUS_NOT_FOUND, Z_MIME_PLAIN_TEXT, "Not found");
}

namespace zroutes {
void notFound()
{
    zserverApp.onNotFound(handleNotFound);
}
}
