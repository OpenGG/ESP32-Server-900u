#pragma once

namespace zusbPin {
void setup();
void loop();
bool enable(char* msg, size_t n);
bool disable(char* msg, size_t n);
}