#include "zconfig.h"
#include "zdebug.h"
#include "zdns.h"
#include "zfs.h"
#include "zserver.h"
#include "zsleep.h"
#include "zwifi.h"

int sleepTime = 0;

void setup()
{
    zdebugBegin(115200);
    zdebug("setup()");

    zdebugDelay(3000);

    zfs.begin(true);
    zconfig::setup();

    IPAddress ip = zwifi::setup();

    zserver::setup();

    zdns::setup(ip);

    int deviceSleep = zconfig::getInt("device_sleep", 30);

    zdebug("deviceSleep: ", deviceSleep);

    if (deviceSleep > 0) {
        sleepTime = millis() + (deviceSleep * 60 * 1000);
    }

    zdebug("setup() end");
}

// long counter = 0;

void loop()
{
    // counter += 1;
    // zdebug("loop(): ", counter);

    zserver::loop();

    zdns::loop();

    // delay(1000);

    if (sleepTime > 0 && millis() > sleepTime) {
        zsleep::sleep();
    }

    zsleep::loop();
}
