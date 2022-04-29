#include "esp_sleep.h"
#include "zconfig.h"
#include "zdebug.h"
#include <Arduino.h>

int sleepTime = 0;

static bool shouldSleep = false;

static void doSleep()
{
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_deep_sleep_start();
}

namespace zsleep {
void setSleep(int seconds)
{
    sleepTime = millis() + (seconds * 1000);
}

void setup()
{
    int deviceSleep = zconfig::getInt("device_sleep", 30);

    zdebug("deviceSleep: ", deviceSleep);

    if (deviceSleep > 0) {
        zsleep::setSleep(deviceSleep * 60);
    }
}

void sleep()
{
    shouldSleep = true;
}

void loop()
{
    if (sleepTime > 0 && millis() > sleepTime) {
        shouldSleep = true;
    }

    if (!shouldSleep) {
        return;
    }

    shouldSleep = false;

    zdebug("Going to sleep");

    delay(1000);
    doSleep();
}
}