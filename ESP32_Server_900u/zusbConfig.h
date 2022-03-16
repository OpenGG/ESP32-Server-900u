#pragma once

#include <Arduino.h>

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3

#define Z_USB_MSC 1

#endif

#define Z_USB_TTL 15000