#include "zwifi.h"
#include "zserver.h"
#include "zconfig.h"
#include "zdebug.h"
#include "zfs.h"

#define Z_SETUP_DELAY 1000

#ifdef Z_DEBUG_ENABLE
#define Z_SETUP_DELAY 3000
#endif

void setup()
{
  delay(Z_SETUP_DELAY);

  zdebugBegin(115200);
  zdebug("setup()");

  zfs.begin(true);

  zconfig::setup();

  zwifi::setup();

  zserver::setup();

  zdebug("setup() end");
}

// long counter = 0;

void loop()
{
  // counter += 1;
  // zdebug("loop(): " + String(counter));
  zconfig::loop();

  zserver::loop();

  // delay(1000);
}
