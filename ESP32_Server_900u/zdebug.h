#pragma once

// #define Z_DEBUG_ENABLE 1

#ifdef Z_DEBUG_ENABLE
  #define zdebugBegin(p) Serial.begin(p);
  #define zdebug(...) Serial.println(__VA_ARGS__);
#else
  #define zdebugBegin(p)
  #define zdebug(...)
#endif
