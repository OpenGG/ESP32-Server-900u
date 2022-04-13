#include <stddef.h>

void zmsg(char* dst, char const* src, size_t max)
{
    if (max == 0) {
        return;
    }
    for (size_t a = 0; a < max; ++a) {
        dst[a] = src[a];
        if (src[a] == 0) {
            // src <= dst
            return;
        }
    }
    // src > dst
    dst[max - 1] = 0;
}
