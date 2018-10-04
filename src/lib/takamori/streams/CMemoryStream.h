#pragma once

#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CMemoryStream : public CStream {

    __extends(CStream, CMemoryStream);

    public:
        CMemoryStream();

        explicit CMemoryStream(uint64_t capacity);

        CMemoryStream(uint64_t capacity, bool_t isResizable);

        CMemoryStream(uint8_t *buffer, uint64_t bufferSize);

        CMemoryStream(uint8_t *buffer, uint64_t bufferSize, bool_t isWritable);

        virtual ~CMemoryStream();

        virtual uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        virtual uint32_t Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        virtual bool_t IsWritable() const override;

        virtual bool_t IsReadable() const override;

        virtual bool_t IsSeekable() const override;

        virtual uint64_t GetPosition() override;

        virtual void SetPosition(uint64_t value) override;

        virtual uint64_t GetLength() override;

        virtual void SetLength(uint64_t value) override;

        virtual void Flush() override;

        virtual uint64_t GetCapacity() const;

        virtual void SetCapacity(uint64_t value);

        virtual void TrimExcess();

        virtual uint8_t *GetBuffer() const;

        virtual const uint8_t *ToArray();

    private:

        CMemoryStream(const CMemoryStream &) = delete;

        bool_t IsResizable() const;

        bool_t IsExternalBuffer() const;

        void EnsureCapacity(uint64_t requestedLength);

    private:

        uint8_t *_buffer;
        bool_t _isResizable;
        bool_t _isExternalBuffer;
        uint64_t _position;
        uint64_t _length;
        uint64_t _capacity;
        bool_t _isWritable;

    };

CGSS_NS_END
