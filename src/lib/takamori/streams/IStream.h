#pragma once

#include "../../cgss_env.h"
#include "../../cgss_enum.h"

CGSS_NS_BEGIN

    struct IStream {

        IStream(IStream &) = delete;

        virtual ~IStream() = default;

        virtual uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) PURE;

        virtual uint32_t Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) PURE;

        virtual void Seek(int64_t offset, StreamSeekOrigin origin) PURE;

        virtual bool_t IsWritable() const PURE;

        virtual bool_t IsReadable() const PURE;

        virtual bool_t IsSeekable() const PURE;

        virtual uint64_t GetPosition() PURE;

        virtual void SetPosition(uint64_t value) PURE;

        virtual uint64_t GetLength() PURE;

        virtual void SetLength(uint64_t value) PURE;

        virtual uint8_t ReadByte() PURE;

        virtual uint32_t WriteByte(uint8_t value) PURE;

        virtual void Flush() PURE;

        virtual void CopyTo(IStream &destination) PURE;

        virtual void CopyTo(IStream &destination, uint32_t bufferSize) PURE;

    protected:
        IStream() = default;

    };

CGSS_NS_END
