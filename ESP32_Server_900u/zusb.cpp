#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32S3)

#include "zconfig.h"
#include <Arduino.h>

#define Z_MAX_USB_TTL 15000

static int getPin()
{
    // set the pin you want to use for usb control
    return zconfig::get("usb_pin", "4").toInt();
}

static long enTime = 0;
static bool hasEnabled = false;

namespace zusb {
void setup()
{
    pinMode(getPin(), OUTPUT);
}

String enable()
{
    if (hasEnabled) {
        return "Usb already enabled";
    }

    digitalWrite(getPin(), HIGH);
    enTime = millis();
    hasEnabled = true;

    return "";
}

String disable()
{
    if (!hasEnabled) {
        return "Usb not enabled";
    }

    enTime = 0;
    hasEnabled = false;
    digitalWrite(getPin(), LOW);
    return "";
}

void loop()
{
    if (hasEnabled && millis() >= (enTime + Z_MAX_USB_TTL)) {
        zusb::disable();
    }
}
}

#endif
