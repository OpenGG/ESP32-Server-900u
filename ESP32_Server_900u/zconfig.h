#pragma once
#include <Arduino.h>

namespace zconfig {
void setup();

String get(
    const char* key,
    const String& defaultValue);

void clear();
}