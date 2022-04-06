#include "zusbConfig.h"

#ifndef Z_USB_MSC

#include "zconfig.h"

static int pin = 4;

static long enTime = 0;
static bool hasEnabled = false;

namespace zusbPin {
void setup()
{
    pin = zconfig::get("usb_pin", "4").toInt();
    pinMode(pin, OUTPUT);
}

String enable()
{
    if (hasEnabled) {
        return "Usb already enabled";
    }

    digitalWrite(pin, HIGH);
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
    digitalWrite(pin, LOW);
    return "";
}

void loop()
{
    if (hasEnabled && millis() >= (enTime + Z_USB_TTL)) {
        zusbPin::disable();
    }
}
}

#endif
