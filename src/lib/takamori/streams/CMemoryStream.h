#pragma once

#include "CStream.h"

#if defined(__CGSS_WITH_JNI__)

#include <jni.h>

#endif

CGSS_NS_BEGIN

    class CGSS_EXPORT CMemoryStream final : public CStream {

    __extends(CStream, CMemoryStream);

    public:
        CMemoryStream();

        explicit CMemoryStream(uint64_t capacity);

        CMemoryStream(uint64_t capacity, bool_t isResizable);

        CMemoryStream(uint8_t *buffer, uint64_t bufferSize);

        CMemoryStream(uint8_t *buffer, uint64_t bufferSize, bool_t isWritable);

        CMemoryStream(const CMemoryStream &) = delete;

        CMemoryStream(CMemoryStream &&) = delete;

        CMemoryStream &operator=(const CMemoryStream &) = delete;

        CMemoryStream &operator=(CMemoryStream &&) = delete;

        ~CMemoryStream() override;

        uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        uint32_t Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        bool_t IsWritable() const override;

        bool_t IsReadable() const override;

        bool_t IsSeekable() const override;

        uint64_t GetPosition() override;

        void SetPosition(uint64_t value) override;

        uint64_t GetLength() override;

        void SetLength(uint64_t value) override;

        void Flush() override;

        virtual uint64_t GetCapacity() const;

        virtual void SetCapacity(uint64_t value);

        virtual void TrimExcess();

        virtual uint8_t *GetBuffer() const;

        virtual const uint8_t *ToArray();

#if defined(__CGSS_WITH_JNI__)
        jobject javaBuffer;
#endif

    private:

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
