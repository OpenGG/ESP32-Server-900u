#pragma once

#if USEFAT
#include "FFat.h"
#define zfs FFat
#else
#include "LittleFS.h"
#define zfs LittleFS
#endif
