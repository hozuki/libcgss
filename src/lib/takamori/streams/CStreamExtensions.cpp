#include "CStreamExtensions.h"

using namespace cgss;

void CStreamExtensions::ReadNullEndedString(IStream *stream, char *buffer, size_t bufferSize) {
    uint8_t t = 1;
    uint32_t i = 0;

    while (t && i < bufferSize) {
        t = stream->ReadByte();

        buffer[i] = (char)t;

        ++i;
    }

    buffer[bufferSize - 1] = '\0';
}
