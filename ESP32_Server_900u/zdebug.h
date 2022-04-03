#pragma once

#if defined(CORE_DEBUG_LEVEL) && CORE_DEBUG_LEVEL != 0
#define Z_EXPAND(x) x
#define Z_FOR_EACH_1(what, x, ...) what(x)
#define Z_FOR_EACH_2(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_1(what, __VA_ARGS__))
#define Z_FOR_EACH_3(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_2(what, __VA_ARGS__))
#define Z_FOR_EACH_4(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_3(what, __VA_ARGS__))
#define Z_FOR_EACH_5(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_4(what, __VA_ARGS__))
#define Z_FOR_EACH_6(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_5(what, __VA_ARGS__))
#define Z_FOR_EACH_7(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_6(what, __VA_ARGS__))
#define Z_FOR_EACH_8(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_7(what, __VA_ARGS__))
#define Z_FOR_EACH_9(what, x, ...) \
    what(x);                       \
    Z_EXPAND(Z_FOR_EACH_8(what, __VA_ARGS__))
#define Z_FOR_EACH_10(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_9(what, __VA_ARGS__))
#define Z_FOR_EACH_11(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_10(what, __VA_ARGS__))
#define Z_FOR_EACH_12(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_11(what, __VA_ARGS__))
#define Z_FOR_EACH_13(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_12(what, __VA_ARGS__))
#define Z_FOR_EACH_14(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_13(what, __VA_ARGS__))
#define Z_FOR_EACH_15(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_14(what, __VA_ARGS__))
#define Z_FOR_EACH_16(what, x, ...) \
    what(x);                        \
    Z_EXPAND(Z_FOR_EACH_15(what, __VA_ARGS__))

#define Z_FOR_EACH_NARG(...) Z_FOR_EACH_NARG_(__VA_ARGS__, Z_FOR_EACH_RSEQ_N())
#define Z_FOR_EACH_NARG_(...) Z_EXPAND(Z_FOR_EACH_ARG_N(__VA_ARGS__))
#define Z_FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define Z_FOR_EACH_RSEQ_N() 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define Z_CONCATENATE(x, y) x##y
#define Z_FOR_EACH_(N, what, ...) Z_EXPAND(Z_CONCATENATE(Z_FOR_EACH_, N)(what, __VA_ARGS__))

#define Z_FOR_EACH(what, ...) Z_FOR_EACH_(Z_FOR_EACH_NARG(__VA_ARGS__), what, __VA_ARGS__)

#define Z_PRINT(x) Serial.print(x)

#define zdebug(...)                     \
    Z_FOR_EACH(Z_PRINT, ##__VA_ARGS__); \
    Serial.println(F(""))

#define zdebugBegin(p) Serial.begin(p);
#define zdebugDelay(dur) delay(dur);
#else
#define zdebugBegin(p)
#define zdebug(...)
#define zdebugDelay(dur)
#endif
