#ifndef __MINGW_H

#include <algorithm>

#endif

using namespace std;

// TODO: FILE: size_t vs. uint64

#include "CMemoryStream.h"
#include "../exceptions/CArgumentException.h"
#include "../exceptions/CInvalidOperationException.h"

CGSS_NS_BEGIN

    static const float MemoryStreamGrowFactor = 1.25f;

    CMemoryStream::CMemoryStream()
        : MyClass(0) {
    }

    CMemoryStream::CMemoryStream(uint64_t capacity)
        : MyClass(capacity, TRUE) {
    }

    CMemoryStream::CMemoryStream(uint64_t capacity, bool_t isResizable) {
        auto buffer = new uint8_t[capacity];
        memset(buffer, 0, (size_t)capacity);
        _buffer = buffer;
        _length = 0;
        _capacity = capacity;
        _isWritable = TRUE;
        _isResizable = isResizable;
        _isExternalBuffer = FALSE;
        _position = 0;
    }

    CMemoryStream::CMemoryStream(uint8_t *buffer, uint64_t bufferSize)
        : MyClass(buffer, bufferSize, TRUE) {
    }

    CMemoryStream::CMemoryStream(uint8_t *buffer, uint64_t bufferSize, bool_t isWritable) {
        _buffer = buffer;
        _length = _capacity = bufferSize;
        _isWritable = isWritable;
        _isResizable = FALSE;
        _isExternalBuffer = TRUE;
        _position = 0;
    }

    CMemoryStream::~CMemoryStream() {
        if (!IsExternalBuffer() && _buffer) {
            delete[] _buffer;
        }
        _buffer = nullptr;
    }

    uint32_t CMemoryStream::Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        if (!buffer) {
            throw CArgumentException("MemoryStream::Read()");
        }
        if (!IsReadable()) {
            throw CInvalidOperationException("MemoryStream::Read()");
        }
        count = min(static_cast<uint32_t>(bufferSize - offset), count);
        size_t maxRead = 0;
        if (count > 0) {
            auto position = GetPosition();
            maxRead = static_cast<size_t>(min(GetLength() - position, (uint64_t)count));
            if (maxRead > 0) {
                const auto byteBuffer = static_cast<uint8_t *>(buffer);
                memcpy(byteBuffer + offset, _buffer + position, maxRead);
                position += maxRead;
                SetPosition(position);
            }
        }
        return static_cast<uint32_t>(maxRead);
    }

    uint32_t CMemoryStream::Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        if (!buffer) {
            throw CArgumentException("MemoryStream::Write()");
        }
        if (!IsWritable()) {
            throw CInvalidOperationException("MemoryStream::Write()");
        }
        count = min(static_cast<uint32_t>(bufferSize - offset), count);
        if (count > 0) {
            auto position = GetPosition();
            const auto expectedLength = position + count;
            EnsureCapacity(expectedLength);
            const auto byteBuffer = static_cast<const uint8_t *>(buffer);
            memcpy(_buffer + position, byteBuffer + offset, count);
            position += count;
            if (position > GetLength()) {
                SetLength(position);
            }
            SetPosition(position);
        }
        return count;
    }

    bool_t CMemoryStream::IsWritable() const {
        return _isWritable;
    }

    bool_t CMemoryStream::IsReadable() const {
        return TRUE;
    }

    bool_t CMemoryStream::IsSeekable() const {
        return TRUE;
    }

    uint64_t CMemoryStream::GetPosition() {
        return _position;
    }

    void CMemoryStream::SetPosition(uint64_t value) {
        EnsureCapacity(value);
        _position = value;
    }

    uint64_t CMemoryStream::GetLength() {
        return _length;
    }

    void CMemoryStream::SetLength(uint64_t value) {
        EnsureCapacity(value);
        auto newLength = value;
        auto oldLength = _length;
        if (newLength < oldLength) {
            auto diff = (size_t)(oldLength - newLength);
            memset(_buffer + newLength, 0, diff);
        }
        _length = newLength;
    }

    void CMemoryStream::Flush() {
        // Do nothing.
    }

    uint64_t CMemoryStream::GetCapacity() const {
        return _capacity;
    }

    void CMemoryStream::SetCapacity(uint64_t value) {
        if (value == _capacity) {
            return;
        }
        if (!IsResizable()) {
            throw CInvalidOperationException("MemoryStream::SetCapacity()");
        }
        auto newBuffer = new uint8_t[value];
        auto oldBuffer = _buffer;
        auto length = (size_t)GetLength();
        auto newCapacity = (size_t)value;
        length = min(length, newCapacity);
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

    void CMemoryStream::TrimExcess() {
        SetCapacity(GetLength());
    }

    uint8_t *CMemoryStream::GetBuffer() const {
        return _buffer;
    }

    const uint8_t *CMemoryStream::ToArray() {
        auto length = (size_t)GetLength();
        uint8_t *p = new uint8_t[length];
        memcpy(p, _buffer, length);
        return p;
    }

    bool_t CMemoryStream::IsResizable() const {
        return _isResizable;
    }

    bool_t CMemoryStream::IsExternalBuffer() const {
        return _isExternalBuffer;
    }

    void CMemoryStream::EnsureCapacity(uint64_t requestedLength) {
        auto capacity = GetCapacity();
        if (capacity >= requestedLength) {
            return;
        }
        if (!IsResizable()) {
            throw CInvalidOperationException("MemoryStream::EnsureCapacity()");
        }
        do {
            capacity = (uint64_t)(capacity * MemoryStreamGrowFactor);
        } while (capacity < requestedLength);
        SetCapacity(capacity);
    }

CGSS_NS_END
