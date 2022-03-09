#include "WiFi.h"
#include "zconfig.h"
#include "zdebug.h"

static IPAddress Server_IP(10, 1, 1, 1);
static IPAddress Subnet_Mask(255, 255, 255, 0);

static IPAddress start(
    const String &ssid,
    const String &pass,
    const String &ip,
    int ch)
{
    Server_IP.fromString(ip);
    // don't set ESP32 as access point and wifi scanner at the same time
    WiFi.mode(WIFI_AP);

    // ps mode
    // zdebug("disable ps mode");
    // WiFi.setSleep(false);

    // launch the access point
    WiFi.softAP(ssid.c_str(), pass.c_str(), ch);

    // important delay, if it's not here the library has not time to set the IP as wanted
    // zdebug("Wait 100 ms for AP_START...");
    // delay(100);

    zdebug("Setting the AP");
    WiFi.softAPConfig(Server_IP, Server_IP, Subnet_Mask);

    // print the IP of access point
    zdebug("Current ip: " + WiFi.softAPIP().toString());

    return Server_IP;
}

static IPAddress connect(
    const String &ssid,
    const String &pass,
    const String &hostname)
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.hostname(hostname);
    WiFi.begin(ssid.c_str(), pass.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        // HWSerial.println("Wifi failed to connect");
    }
    else
    {
        IPAddress LAN_IP = WiFi.localIP();

        return LAN_IP;
    }

    return Server_IP;
}

namespace zwifi
{
    IPAddress setup()
    {
        String wifi_connect = zconfig::get("wifi_connect", "0");

        String wifi_ap_ssid = zconfig::get("wifi_ap_ssid", "PS4_WEB_AP");
        String wifi_ap_pass = zconfig::get("wifi_ap_pass", "password");
        String wifi_ap_ip = zconfig::get("wifi_ap_ip", "10.1.1.1");
        int wifi_ap_ch = zconfig::get("wifi_ap_ch", "11").toInt();

        String wifi_ssid = zconfig::get("wifi_ssid", "Home_WIFI");
        String wifi_pass = zconfig::get("wifi_pass", "password");
        String wifi_hostname = zconfig::get("wifi_hostname", "ps4.local");

        zdebug("wifi_connect: " + wifi_connect);
        zdebug("wifi_ap_ssid: " + wifi_ap_ssid);
        zdebug("wifi_ap_pass: " + wifi_ap_pass);
        zdebug("wifi_ap_ip: " + wifi_ap_ip);
        zdebug("wifi_ap_ch: " + String(wifi_ap_ch));

        bool apMode = wifi_connect.equals("0");

        if (apMode)
        {
            return start(wifi_ap_ssid, wifi_ap_pass, wifi_ap_ip, wifi_ap_ch);
        }

        return connect(wifi_ssid, wifi_pass, wifi_hostname);
    }
}