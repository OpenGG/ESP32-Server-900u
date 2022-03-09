#pragma once

namespace zconfig {
    String get(
        const String &key,
        const String &defaultValue
    );

    void set(
        const String &key,
        const String &value
    );

    void clear();

    void setup();

    void loop();
}