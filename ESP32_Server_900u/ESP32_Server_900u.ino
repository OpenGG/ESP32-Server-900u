#include "zconfig.h"
#include "zdebug.h"
#include "zdns.h"
#include "zfs.h"
#include "zserver.h"
#include "zwifi.h"

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

    zdebug("setup() end");
}

// long counter = 0;

void loop()
{
    // counter += 1;
    // zdebug("loop(): " + String(counter));
    zconfig::loop();

    zserver::loop();

    zdns::loop();

    // delay(1000);
}
