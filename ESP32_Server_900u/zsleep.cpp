#include "esp_sleep.h"
#include "zdebug.h"
#include <Arduino.h>

static bool shouldSleep = false;

static void doSleep()
{
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_deep_sleep_start();
}

namespace zsleep {
void sleep()
{
    shouldSleep = true;
}

void loop()
{
    if (!shouldSleep) {
        return;
    }

    shouldSleep = false;

    zdebug("Going to sleep");

    delay(1000);
    doSleep();
}
}