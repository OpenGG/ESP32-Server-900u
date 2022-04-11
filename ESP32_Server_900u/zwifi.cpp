#include "WiFi.h"
#include "zconfig.h"
#include "zdebug.h"

static IPAddress Server_IP(10, 1, 1, 1);
static IPAddress Subnet_Mask(255, 255, 255, 0);

static IPAddress start(
    char const* ssid,
    char const* pass,
    char const* ip,
    int ch)
{
    Server_IP.fromString(ip);
    // don't set ESP32 as access point and wifi scanner at the same time
    WiFi.mode(WIFI_AP);

    // ps mode
    // zdebug("disable ps mode");
    // WiFi.setSleep(false);

    // launch the access point
    WiFi.softAP(ssid, pass, ch);

    // important delay, if it's not here the library has not time to set the IP as wanted
    // zdebug("Wait 100 ms for AP_START...");
    // delay(100);

    zdebug("Setting the AP");
    WiFi.softAPConfig(Server_IP, Server_IP, Subnet_Mask);

    // print the IP of access point
    zdebug("Current ip: ", WiFi.softAPIP().toString());

    return Server_IP;
}

static IPAddress connect(
    char const* ssid,
    char const* pass,
    char const* hostname)
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.setHostname(hostname);
    WiFi.begin(ssid, pass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        // HWSerial.println("Wifi failed to connect");
    } else {
        IPAddress LAN_IP = WiFi.localIP();

        return LAN_IP;
    }

    return Server_IP;
}

namespace zwifi {
IPAddress setup()
{
    int wifi_connect = zconfig::getInt("wifi_connect", 0);

    char const* wifi_ap_ssid = zconfig::get("wifi_ap_ssid", "ESP_WEB_AP");
    char const* wifi_ap_pass = zconfig::get("wifi_ap_pass", "123456789");
    char const* wifi_ap_ip = zconfig::get("wifi_ap_ip", "10.1.1.1");
    int wifi_ap_ch = zconfig::getInt("wifi_ap_ch", 11);

    char const* wifi_ssid = zconfig::get("wifi_ssid", "Home_WIFI");
    char const* wifi_pass = zconfig::get("wifi_pass", "password");
    char const* wifi_hostname = zconfig::get("wifi_hostname", "esp.local");

    zdebug("wifi_connect: ", wifi_connect);
    zdebug("wifi_ap_ssid: ", wifi_ap_ssid);
    zdebug("wifi_ap_pass: ", wifi_ap_pass);
    zdebug("wifi_ap_ip: ", wifi_ap_ip);
    zdebug("wifi_ap_ch: ", wifi_ap_ch);

    bool apMode = wifi_connect == 0;

    if (apMode) {
        return start(wifi_ap_ssid, wifi_ap_pass, wifi_ap_ip, wifi_ap_ch);
    }

    return connect(wifi_ssid, wifi_pass, wifi_hostname);
}
}