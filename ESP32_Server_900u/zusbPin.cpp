#include "zusbConfig.h"

#ifndef Z_USB_MSC

#include "zconfig.h"
#include "zdebug.h"

static int pin = 4;

static long enTime = 0;
static bool hasEnabled = false;

static void doDisable()
{
    digitalWrite(pin, LOW);
}

namespace zusbPin {
void setup()
{
    pin = zconfig::getInt("usb_pin", 4);
    pinMode(pin, OUTPUT);
}

bool enable(char* msg, size_t n)
{
    if (hasEnabled) {
        strncpy(msg, "Usb already enabled", n - 1);
        return false;
    }

    digitalWrite(pin, HIGH);
    enTime = millis();
    hasEnabled = true;

    return true;
}

bool disable(char* msg, size_t n)
{
    if (!hasEnabled) {
        strncpy(msg, "Usb not enabled", n - 1);
        return false;
    }

    enTime = 0;
    hasEnabled = false;
    doDisable();
    return true;
}

void loop()
{
    if (hasEnabled && millis() >= (enTime + Z_USB_TTL)) {
        hasEnabled = false;

        zdebug("zusbPin::disable(): auto");

        doDisable();
    }
}
}

#endif
