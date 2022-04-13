#include "zcfg.h"
#include <Arduino.h>

namespace zconfig {
void setup()
{
    zcfg::begin();
}

char const* get(
    char const* key,
    char const* defaultValue)
{
    return zcfg::get(key, defaultValue);
}

int getInt(
    char const* key,
    int defaultValue)
{
    return zcfg::getInt(key, defaultValue);
}

void clear()
{
    zcfg::clear();
}
}