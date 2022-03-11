#pragma once

#if defined(CORE_DEBUG_LEVEL) && CORE_DEBUG_LEVEL != 0
#define zdebugBegin(p) Serial.begin(p);
#define zdebug(...) Serial.println(__VA_ARGS__);
#define zdebugDelay(dur) delay(dur);
#else
#define zdebugBegin(p)
#define zdebug(...)
#define zdebugDelay(dur)
#endif
