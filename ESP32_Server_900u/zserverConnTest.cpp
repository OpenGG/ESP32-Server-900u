#include "ESPAsyncWebServer.h"
#include "zserverApp.h"

static void handleConnTest(AsyncWebServerRequest* request)
{
    request->send(Z_STATUS_OK, Z_MIME_PLAIN_TEXT, "Microsoft Connect Test");
}

namespace zroutes {
void connTest()
{
    zserverApp.on("/connecttest.txt", HTTP_GET, handleConnTest);
}
}
