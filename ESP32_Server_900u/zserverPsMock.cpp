#include "ESPAsyncWebServer.h"
#include "zserverApp.h"
#include "zdebug.h"

#define Version "05.050.000"
#define sVersion Version
#define lblVersion "5.05"
#define imgSize "0"
#define imgPath ""

static String split(String str, String from, String to)
{
    String tmpstr = str;
    tmpstr.toLowerCase();
    from.toLowerCase();
    to.toLowerCase();
    int pos1 = tmpstr.indexOf(from);
    if (pos1 == -1)
    {
        return "";
    }

    int pos2 = tmpstr.indexOf(to, pos1 + from.length());
    if (pos2 == -1)
    {
        return "";
    }

    String retval = str.substring(pos1 + from.length(), pos2);
    return retval;
}

class OneParamRewrite : public AsyncWebRewrite
{
protected:
    String _urlPrefix;
    int _paramIndex;
    String _paramsBackup;

public:
    OneParamRewrite(const char *from, const char *to)
        : AsyncWebRewrite(from, to)
    {

        _paramIndex = _from.indexOf('{');

        if (_paramIndex >= 0 && _from.endsWith("}"))
        {
            _urlPrefix = _from.substring(0, _paramIndex);
            int index = _params.indexOf('{');
            if (index >= 0)
            {
                _params = _params.substring(0, index);
            }
        }
        else
        {
            _urlPrefix = _from;
        }
        _paramsBackup = _params;
    }

    bool match(AsyncWebServerRequest *request) override
    {
        if (request->url().startsWith(_urlPrefix))
        {
            if (_paramIndex >= 0)
            {
                _params = _paramsBackup + request->url().substring(_paramIndex);
            }
            else
            {
                _params = _paramsBackup;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
};

static void handleConsoleUpdate(AsyncWebServerRequest *request)
{
    String rgn = "cn";

    if (request->hasParam("p"))
    {
        AsyncWebParameter *p = request->getParam("p");
        String name = p->name();

        rgn = split(name, "list/", "/");
        zdebug("/update/ps4/" + rgn);
    }

    int params = request->params();
    String xmlStr = "<?xml version=\"1.0\" ?><update_data_list><region id=\"" + rgn + "\"><force_update><system level0_system_ex_version=\"0\" level0_system_version=\"" + Version + "\" level1_system_ex_version=\"0\" level1_system_version=\"" + Version + "\"/></force_update><system_pup ex_version=\"0\" label=\"" + lblVersion + "\" sdk_version=\"" + sVersion + "\" version=\"" + Version + "\"><update_data update_type=\"full\"><image size=\"" + imgSize + "\">" + imgPath + "</image></update_data></system_pup><recovery_pup type=\"default\"><system_pup ex_version=\"0\" label=\"" + lblVersion + "\" sdk_version=\"" + sVersion + "\" version=\"" + Version + "\"/><image size=\"" + imgSize + "\">" + imgPath + "</image></recovery_pup></region></update_data_list>";

    request->send(200, "text/xml", xmlStr);
}

namespace zroutes
{
    void psMock()
    {
        zserverApp.addRewrite(
            new OneParamRewrite("/update/ps4/{p}", "/update/ps4?p={p}"));

        zserverApp.on(
            "/update/ps4", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleConsoleUpdate(request); });
    }
}