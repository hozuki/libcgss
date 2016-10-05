#ifndef CGSS_MEMORYSTREAM_H
#define CGSS_MEMORYSTREAM_H

#include "tm_stream.h"

class MemoryStream final : public Stream {

public:

    MemoryStream();

    MemoryStream(uint64 capacity);

    MemoryStream(uint8 *buffer, uint64 bufferSize);

    MemoryStream(uint8 *buffer, uint64 bufferSize, ubool isWritable);

    ~MemoryStream();

    virtual uint32 Read(uint8 *buffer, uint32 bufferSize, size_t offset, uint32 count) override;

    virtual uint32 Write(const uint8 *buffer, uint32 bufferSize, uint32 offset, uint32 count) override;

    virtual ubool IsWritable() const override;

    virtual ubool IsReadable() const override;

    virtual ubool IsSeekable() const override;

    virtual uint64 GetPosition() const override;

    virtual void SetPosition(uint64 value) override;

    virtual uint64 GetLength() const override;

    virtual void SetLength(uint64 value) override;

    virtual void Flush() override;

    uint64 GetCapacity() const;

    void SetCapacity(uint64 value);

    void TrimExcess();

    uint8 *GetBuffer() const;

    const uint8 *ToArray() const;

private:

    ubool IsResizable() const;

    ubool IsExternalBuffer() const;

    void EnsureCapacity(uint64 requestedLength);

private:

    uint8 *_buffer;
    ubool _isResizable;
    ubool _isExternalBuffer;
    uint64 _position;
    uint64 _length;
    uint64 _capacity;
    ubool _isWritable;

};

#endif //CGSS_MEMORYSTREAM_H
