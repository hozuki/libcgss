#include <stdexcept>
#include "tm_stream.h"

#ifndef __MINGW_H

#include <algorithm>

#endif

Stream::Stream() {
}

Stream::~Stream() {
}

void Stream::Seek(int64 offset, TM_STREAM_SEEK_ORIGIN origin) {
    int64 position;
    switch (origin) {
        case TM_SEEK_BEGIN:
            position = offset;
            break;
        case TM_SEEK_CURRENT:
            position = GetPosition() + offset;
            break;
        case TM_SEEK_END:
            position = GetLength() + offset;
            break;
        default:
            throw std::out_of_range("Stream::Seek()");
    }
    if (position != GetPosition()) {
        SetPosition((uint64)position);
    }
}

uint8 Stream::ReadByte() {
    uint8 b = 0;
    Read(&b, 1, 0, 1);
    return b;
}

void Stream::WriteByte(const uint8 value) {
    Write(&value, 1, 0, 1);
}

void Stream::CopyTo(Stream &destination) {
    CopyTo(destination, 10240);
}

void Stream::CopyTo(Stream &destination, uint32 bufferSize) {
    if (bufferSize == 0) {
        return;
    }
    if (!IsReadable()) {
        return;
    }
    uint8 *buffer = new uint8[bufferSize];
    uint32 read = 1;
    while (read > 0) {
        read = Read(buffer, bufferSize, 0, bufferSize);
        if (read > 0) {
            destination.Write(buffer, bufferSize, 0, read);
        }
        if (read < bufferSize) {
            break;
        }
    }
    delete[] buffer;
}
