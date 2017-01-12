#include "CStream.h"
#include "../exceptions/CException.h"
#include "../exceptions/CArgumentException.h"

CGSS_NS_BEGIN

    void CStream::Seek(int64_t offset, StreamSeekOrigin origin) {
        int64_t position;
        switch (origin) {
            case StreamSeekOrigin::Begin:
                position = offset;
                break;
            case StreamSeekOrigin::Current:
                position = GetPosition() + offset;
                break;
            case StreamSeekOrigin::End:
                position = GetLength() + offset;
                break;
        }
        if (position != GetPosition()) {
            SetPosition((uint64_t)position);
        }
    }

    uint8_t CStream::ReadByte() {
        uint8_t b = 0;
        auto read = Read(&b, 1, 0, 1);
        if (read < 1) {
            throw CException(CGSS_OP_BUFFER_TOO_SMALL);
        }
        return b;
    }

    uint32_t CStream::WriteByte(const uint8_t value) {
        return Write(&value, 1, 0, 1);
    }

    void CStream::CopyTo(IStream &destination) {
        CopyTo(destination, 10240);
    }

    void CStream::CopyTo(IStream &destination, uint32_t bufferSize) {
        if (this == &destination) {
            throw CArgumentException("CStream::CopyTo");
        }
        if (bufferSize == 0) {
            return;
        }
        if (!IsReadable()) {
            return;
        }
        uint8_t *buffer = new uint8_t[bufferSize];
        uint32_t read = 1;
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

CGSS_NS_END
