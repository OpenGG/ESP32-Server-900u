#pragma once
#include <Arduino.h>

namespace zcfg {
void begin();

String get(
    const char* key,
    const String& defaultValue);

void clear();
}
