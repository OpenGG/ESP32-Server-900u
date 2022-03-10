#include "ESPAsyncWebServer.h"
#include "zdebug.h"
#include "zfs.h"
#include "zserverApp.h"

static void handleListFiles(AsyncWebServerRequest* request)
{
    File dir = zfs.open("/");
    String out = "";
    for (File file = dir.openNextFile(); file; file = dir.openNextFile()) {
        String fname = String(file.name());

        out += fname + "\t" + file.size() + "\n";

        file.close();
    }
    dir.close();

    request->send(200, Z_MIME_PLAIN_TEXT, out);
}

static void handleDelete(AsyncWebServerRequest* request)
{
    if (!request->hasParam("file", true)) {
        request->send(400, Z_MIME_PLAIN_TEXT, "file not provided");
        return;
    }
    String path = request->getParam("file", true)->value();
    if (path.length() == 0 || path.equals("/")) {
        request->send(400, Z_MIME_PLAIN_TEXT, "file not valid");
        return;
    }
    if (!path.startsWith("/")) {
        path = "/" + path;
    }
    if (!zfs.exists(path)) {
        request->send(404, Z_MIME_PLAIN_TEXT, "file not exists");
        return;
    }
    zfs.remove(path);
    request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE);
}

File upFile;

static void handleFileUpload(AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final)
{
    zdebug("onUpload: " + request->url() + " " + filename);

    if (!filename.startsWith("/")) {
        filename = "/" + filename;
    }

    if (filename.equals("/")) {
        request->send(400, Z_MIME_PLAIN_TEXT, "file not valid");
        return;
    }

    if (!index) {
        upFile = zfs.open(filename, "w");
    }
    if (upFile) {
        upFile.write(data, len);
    }
    if (final) {
        upFile.close();
    }
}

namespace zroutes {
void files()
{
    zserverApp.on("/admin/files/list", HTTP_GET, handleListFiles);

    zserverApp.on("/admin/files/delete", HTTP_POST, handleDelete);

    zserverApp.on(
        "/admin/files/upload", HTTP_POST, [](AsyncWebServerRequest* request) {
                zdebug("onRequest: " + request->url());
                request->send(200, Z_MIME_PLAIN_TEXT, Z_MSG_DONE); },
        handleFileUpload);
}
}
