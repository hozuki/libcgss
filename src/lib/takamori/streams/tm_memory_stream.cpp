#include <stdexcept>
#include "tm_memory_stream.h"

// TODO: FILE: size_t vs. uint64

static const float MemoryStreamGrowFactor = 1.25f;

MemoryStream::MemoryStream()
        : MemoryStream(0) {
}

MemoryStream::MemoryStream(uint64 capacity) {
    auto buffer = new uint8[capacity];
    memset(buffer, 0, (size_t)capacity);
    _buffer = buffer;
    _length = 0;
    _capacity = capacity;
    _isWritable = TRUE;
    _isResizable = TRUE;
    _isExternalBuffer = FALSE;
    _position = 0;
}

MemoryStream::MemoryStream(uint8 *buffer, uint64 bufferSize)
        : MemoryStream(buffer, bufferSize, TRUE) {
}

MemoryStream::MemoryStream(uint8 *buffer, uint64 bufferSize, ubool isWritable) {
    _buffer = buffer;
    _length = _capacity = bufferSize;
    _isWritable = isWritable;
    _isResizable = FALSE;
    _isExternalBuffer = TRUE;
    _position = 0;
}

MemoryStream::~MemoryStream() {
    if (!IsExternalBuffer() && _buffer) {
        delete[] _buffer;
    }
    _buffer = nullptr;
}

uint32 MemoryStream::Read(uint8 *buffer, uint32 bufferSize, size_t offset, uint32 count) {
    if (!buffer) {
        throw std::invalid_argument("MemoryStream::Read()");
    }
    if (!IsReadable()) {
        throw std::runtime_error("MemoryStream::Read()");
    }
    count = std::min(bufferSize - offset, count);
    size_t maxRead = 0;
    if (count > 0) {
        auto position = (uint64)GetPosition();
        maxRead = (size_t)std::min(GetLength() - position, (uint64)count);
        if (maxRead > 0) {
            memcpy(buffer, _buffer + position, maxRead);
            position += maxRead;
            SetPosition(position);
        }
    }
    return maxRead;
}

uint32 MemoryStream::Write(const uint8 *buffer, uint32 bufferSize, uint32 offset, uint32 count) {
    if (!buffer) {
        throw std::invalid_argument("MemoryStream::Read()");
    }
    if (!IsWritable()) {
        throw std::runtime_error("MemoryStream::Write()");
    }
    count = std::min(bufferSize - offset, count);
    if (count > 0) {
        auto position = (uint64)GetPosition();
        auto expectedLength = position + count;
        EnsureCapacity(expectedLength);
        memcpy(_buffer + position, buffer + offset, count);
        position += count;
        if (position > GetLength()) {
            SetLength(position);
        }
        SetPosition(position);
    }
    return count;
}

ubool MemoryStream::IsWritable() const {
    return _isWritable;
}

ubool MemoryStream::IsReadable() const {
    return TRUE;
}

ubool MemoryStream::IsSeekable() const {
    return TRUE;
}

uint64 MemoryStream::GetPosition() const {
    return _position;
}

void MemoryStream::SetPosition(uint64 value) {
    EnsureCapacity(value);
    _position = value;
}

uint64 MemoryStream::GetLength() const {
    return _length;
}

void MemoryStream::SetLength(uint64 value) {
    EnsureCapacity(value);
    auto newLength = value;
    auto oldLength = _length;
    if (newLength < oldLength) {
        auto diff = (size_t)(oldLength - newLength);
        memset(_buffer + newLength, 0, diff);
    }
    _length = newLength;
}

void MemoryStream::Flush() {
    // Do nothing.
}

uint64 MemoryStream::GetCapacity() const {
    return _capacity;
}

void MemoryStream::SetCapacity(uint64 value) {
    if (value == _capacity) {
        return;
    }
    if (!IsResizable()) {
        throw std::runtime_error("MemoryStream::SetCapacity()");
    }
    auto newBuffer = new uint8[value];
    auto oldBuffer = _buffer;
    auto length = (size_t)GetLength();
    auto newCapacity = (size_t)value;
    length = std::min(length, newCapacity);
    if (length > 0) {
        memcpy(newBuffer, oldBuffer, length);
        if (newCapacity > length) {
            auto diff = newCapacity - length;
            memset(newBuffer + length, 0, diff);
        }
    }
    _buffer = newBuffer;
    delete[] oldBuffer;
    _capacity = newCapacity;
}

void MemoryStream::TrimExcess() {
    SetCapacity(GetLength());
}

uint8 *MemoryStream::GetBuffer() const {
    return _buffer;
}

const uint8 *MemoryStream::ToArray() const {
    auto length = (size_t)GetLength();
    uint8 *p = new uint8[length];
    memcpy(p, _buffer, length);
    return p;
}

ubool MemoryStream::IsResizable() const {
    return _isResizable;
}

ubool MemoryStream::IsExternalBuffer() const {
    return _isExternalBuffer;
}

void MemoryStream::EnsureCapacity(uint64 requestedLength) {
    auto capacity = GetCapacity();
    if (capacity >= requestedLength) {
        return;
    }
    if (!IsResizable()) {
        throw std::runtime_error("MemoryStream::EnsureCapacity()");
    }
    do {
        capacity = (uint64)(capacity * MemoryStreamGrowFactor);
    } while (capacity < requestedLength);
    SetCapacity(capacity);
}
