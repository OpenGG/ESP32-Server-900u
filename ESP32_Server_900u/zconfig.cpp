#include <Arduino.h>
#include <Preferences.h>

static Preferences prefs;

static bool closed = true;

static void ensure()
{
    if (!closed)
    {
        return;
    }

    closed = false;

    prefs.begin("pref", false);
}

static void cleanup()
{
    if (closed)
    {
        return;
    }

    closed = true;

    prefs.end();
}

namespace zconfig
{
    void set(
        const String &key,
        const String &value)
    {
        ensure();

        int len = key.length() + 1;
        char k[len];
        key.toCharArray(k, len);

        prefs.putString(k, value);
    }

    String get(
        const String &key,
        const String &defaultValue)
    {
        ensure();

        int len = key.length() + 1;
        char k[len];
        key.toCharArray(k, len);

        return prefs.getString(k, defaultValue);
    }

    void clear()
    {
        ensure();

        prefs.clear();
    }

    void setup()
    {
    }

    void loop()
    {
        cleanup();
    }
}