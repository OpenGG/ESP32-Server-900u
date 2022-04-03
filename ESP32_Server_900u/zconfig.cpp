#include "zcfg.h"
#include <Arduino.h>

namespace zconfig {
void setup()
{
    zcfg::begin();
}

String get(
    const char* key,
    const String& defaultValue)
{
    return zcfg::get(key, defaultValue);
}

void clear()
{
    zcfg::clear();
}
}