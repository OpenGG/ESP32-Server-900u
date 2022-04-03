#include "zdebug.h"
#include "zfs.h"

#define Z_CONFIG_RESULT_MAX 4096
#define Z_CONFIG_BUFF_SIZE 128
#define Z_CONFIG_FILE_PATH "/config.txt"

static char buff[Z_CONFIG_BUFF_SIZE] = { 0 };
static char result[Z_CONFIG_RESULT_MAX] = { 0 };

static void trim(int& start, int& end)
{
    for (int i = start; i < end; ++i) {
        // printf("l %d\n", buff[i]);
        if (buff[i] == ' ') {
            start = i + 1;
        } else {
            break;
        }
    }

    for (int i = end - 1; i > start; --i) {
        // printf("r %d\n", buff[i]);

        if (buff[i] == ' ') {
            end = i;
        } else {
            break;
        }
    }
}

static void emit(int& cursor, int& count, int keyStart, int keyEnd, int valueStart, int valueEnd)
{
    trim(keyStart, keyEnd);
    trim(valueStart, valueEnd);

    zdebug(
        "emit",
        " cursor=", cursor,
        " count=", count,
        " keyStart=", keyStart,
        " keyEnd=", keyEnd,
        " valueStart=", valueStart,
        " valueEnd=", valueEnd);

    // zdebug("key:");
    // for (int i = keyStart; i < keyEnd; ++i) {
    //     zdebug(buff[i]);
    // }

    // zdebug("value:");
    // for (int i = valueStart; i < valueEnd; ++i) {
    //     zdebug(buff[i]);
    // }

    int keyLen = keyEnd - keyStart + 1;
    int valueLen = valueEnd - valueStart + 1;

    if (cursor + 1 + keyLen + 1 + valueLen >= Z_CONFIG_RESULT_MAX) {
        return;
    }

    if (count >= 255) {
        return;
    }

    int c = cursor;
    // key size
    result[c] = keyLen;
    c += 1;

    // key
    if (keyLen > 1) {
        memcpy(result + c, buff + keyStart, keyLen - 1);
    }
    c += keyLen;

    // value size
    result[c] = valueLen;
    c += 1;

    // value
    if (valueLen > 1) {
        memcpy(result + c, buff + valueStart, valueLen - 1);
    }

    c += valueLen;

    cursor = c;
    count += 1;
    result[0] = count;
}

static void parse()
{
    if (!zfs.exists(Z_CONFIG_FILE_PATH)) {
        zdebug("File not exists: ", Z_CONFIG_FILE_PATH);
        return;
    }

    File file = zfs.open(Z_CONFIG_FILE_PATH, "r");

    if (!file) {
        zdebug("Failed to open: ", Z_CONFIG_FILE_PATH);
        return;
    }

    int keyStart = 0;
    int keyEnd = 0;
    int valueStart = 0;
    int valueEnd = 0;
    int idx = 0;
    int cursor = 1;
    int count = 0;
    char ch = 0;
    bool newlineAdded = false;

    enum STATUS {
        START = 0,
        COMMENT,
        KEY,
        VALUE,
    } status
        = START;

    while (true) {
        if (file.available()) {
            ch = file.read();
        } else if (idx > 0 && !newlineAdded) {
            newlineAdded = true;
            ch = '\n';
        } else {
            break;
        }

        // zdebug("idx=", idx, " ch=", ch, " status=", status);

        idx = min(idx, Z_CONFIG_BUFF_SIZE - 1);

        buff[idx] = ch;

        switch (status) {
        case START:
            switch (ch) {
            case '\n':
            case '\r':
                idx = 0;
                break;
            case ' ':
            case '\t':
                // ignore
                break;
            case '#':
                status = COMMENT;
                break;
            default:
                status = KEY;
                keyStart = idx;
                break;
            }
            break;
        case COMMENT:
            switch (ch) {
            case '\n':
            case '\r':
                if (valueEnd > 0) {
                    emit(cursor, count, keyStart, keyEnd, valueStart, valueEnd);
                }
                keyStart = 0;
                keyEnd = 0;
                valueStart = 0;
                valueEnd = 0;
                idx = 0;
                status = START;
                break;
            default:
                // ignore
                break;
            }
            break;
        case KEY:
            switch (ch) {
            case '=':
                status = VALUE;
                keyEnd = idx;
                valueStart = idx + 1;
                break;
            case '\n':
            case '\r':
                keyStart = 0;
                keyEnd = 0;
                valueStart = 0;
                valueEnd = 0;
                idx = 0;
                status = START;
                break;
            case '#':
                keyStart = 0;
                keyEnd = 0;
                status = COMMENT;
                break;
            }
            break;
        case VALUE:
            switch (ch) {
            case '\n':
            case '\r':
                valueEnd = valueEnd == 0 ? idx : valueEnd;

                emit(cursor, count, keyStart, keyEnd, valueStart, valueEnd);

                keyStart = 0;
                keyEnd = 0;
                valueStart = 0;
                valueEnd = 0;
                idx = 0;

                status = START;
                break;
            case '#':
                valueEnd = valueEnd == 0 ? idx : valueEnd;
                status = COMMENT;
                break;
            default:
                //
                break;
            }
            break;
        }

        idx += 1;
    }

    // zdebug("result");
    // zdebug(result);

    file.close();
}

namespace zcfg {
void begin()
{
    parse();
}

String get(const char* key,
    const String& defaultValue)
{
    int count = result[0];
    int cursor = 1;
    for (int i = 0; i < count; i += 1) {
        int keyLen = result[cursor];
        cursor += 1;

        if (keyLen <= 0 || cursor + keyLen >= Z_CONFIG_RESULT_MAX) {
            break;
        }

        bool isMatch = memcmp(key, result + cursor, keyLen) == 0;
        cursor += keyLen;

        int valueLen = result[cursor];
        cursor += 1;

        if (isMatch) {
            if (valueLen == 0) {
                zdebug("zcfg ", key, ": empty value");
                return "";
            }

            if (valueLen < 0 || cursor + valueLen >= Z_CONFIG_RESULT_MAX) {
                break;
            }

            char v[valueLen];

            memcpy(v, result + cursor, valueLen);

            zdebug("zcfg ", key, ": ", v);

            return String(v);
        }

        cursor += valueLen;
    }

    zdebug("zcfg ", key, ": ", defaultValue, " (default)");

    return defaultValue;
}

void clear()
{
    File file = zfs.open(Z_CONFIG_FILE_PATH, "w");
    file.print("");
    file.close();
    result[0] = 0;
}
}
