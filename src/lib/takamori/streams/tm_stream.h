#ifndef CGSS_STREAM_H
#define CGSS_STREAM_H

#include <stdlib.h>
#include "../../tm_api.h"

typedef enum _TM_STREAM_SEEK_ORIGIN {
    TM_SEEK_BEGIN = 0,
    TM_SEEK_CURRENT = 1,
    TM_SEEK_END = 2
} TM_STREAM_SEEK_ORIGIN;

class Stream {

public:

    Stream();

    virtual ~Stream();

    virtual uint32 Read(uint8 *buffer, uint32 bufferSize, size_t offset, uint32 count) = 0;

    virtual uint32 Write(const uint8 *buffer, uint32 bufferSize, uint32 offset, uint32 count) = 0;

    virtual void Seek(int64 offset, TM_STREAM_SEEK_ORIGIN origin);

    virtual ubool IsWritable() const = 0;

    virtual ubool IsReadable() const = 0;

    virtual ubool IsSeekable() const = 0;

    virtual uint64 GetPosition() const = 0;

    virtual void SetPosition(const uint64 value) = 0;

    virtual uint64 GetLength() const = 0;

    virtual void SetLength(const uint64 value) = 0;

    uint8 ReadByte();

    void WriteByte(uint8 value);

    virtual void Flush() = 0;

    void CopyTo(Stream &destination);

    void CopyTo(Stream &destination, uint32 bufferSize);

};


#endif //CGSS_STREAM_H
