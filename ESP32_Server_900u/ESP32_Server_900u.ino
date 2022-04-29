#include "zconfig.h"
#include "zdebug.h"
#include "zdns.h"
#include "zfs.h"
#include "zserver.h"
#include "zsleep.h"
#include "zwifi.h"

void setup()
{
    zdebugBegin(115200);

    zdebugDelay(3000);

    zdebug("setup()");

    zfs.begin(true);
    zconfig::setup();

    IPAddress ip = zwifi::setup();

    zserver::setup();

    zdns::setup(ip);

    zsleep::setup();

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

    zsleep::loop();
}
