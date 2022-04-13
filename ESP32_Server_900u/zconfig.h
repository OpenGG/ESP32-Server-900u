#pragma once
#include <Arduino.h>

namespace zconfig {
void setup();

char const* get(
    char const* key,
    char const* defaultValue);

int getInt(
    char const* key,
    int defaultValue);

void clear();
}