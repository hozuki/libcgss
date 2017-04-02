#pragma once

#include "../../cgss_env.h"
#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CBinaryWriter : public CStream {

    __extends(CStream, CBinaryWriter);

    public:

        CBinaryWriter(IStream *baseStream);

        uint32_t WriteInt8(int8_t v);

        uint32_t WriteUInt8(uint8_t v);

        uint32_t WriteInt16LE(int16_t v);

        uint32_t WriteInt16BE(int16_t v);

        uint32_t WriteUInt16LE(uint16_t v);

        uint32_t WriteUInt16BE(uint16_t v);

        uint32_t WriteInt32LE(int32_t v);

        uint32_t WriteInt32BE(int32_t v);

        uint32_t WriteUInt32LE(uint32_t v);

        uint32_t WriteUInt32BE(uint32_t v);

        uint32_t WriteInt64LE(int64_t v);

        uint32_t WriteInt64BE(int64_t v);

        uint32_t WriteUInt64LE(uint64_t v);

        uint32_t WriteUInt64BE(uint64_t v);

        uint32_t WriteSingleLE(float v);

        uint32_t WriteSingleBE(float v);

        uint32_t WriteDoubleLE(double v);

        uint32_t WriteDoubleBE(double v);

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

    private:

        CBinaryWriter() = delete;

        CBinaryWriter(const CBinaryWriter &) = delete;

        IStream *_baseStream;

    };

CGSS_NS_END
