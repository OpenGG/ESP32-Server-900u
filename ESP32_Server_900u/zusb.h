#pragma once

#include "zusbConfig.h"
#include "zusbMsc.h"
#include "zusbPin.h"

#ifdef Z_USB_MSC

#define zusb zusbMsc

#else

#define zusb zusbPin

#endif