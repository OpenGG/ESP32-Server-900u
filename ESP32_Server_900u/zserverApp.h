#pragma once

#include "ESPAsyncWebServer.h"

#define Z_STATUS_OK 200
#define Z_STATUS_SERVER_ERROR 503
#define Z_STATUS_CLIENT_ERROR 400
#define Z_STATUS_NOT_FOUND 404
#define Z_MIME_PLAIN_TEXT "text/plain"
#define Z_MIME_HTML "text/html"
#define Z_MIME_XML "text/xml"
#define Z_MSG_DONE "Done"

extern AsyncWebServer zserverApp;
