#include "zusbConfig.h"

#ifndef Z_USB_MSC

#include "zconfig.h"

static int getPin()
{
    // set the pin you want to use for usb control
    return zconfig::get("usb_pin", "4").toInt();
}

static long enTime = 0;
static bool hasEnabled = false;

namespace zusbPin {
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
    if (hasEnabled && millis() >= (enTime + Z_USB_TTL)) {
        zusb::disable();
    }
}
}

#endif
