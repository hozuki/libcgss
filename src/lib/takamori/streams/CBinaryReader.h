#pragma once

#include "../../cgss_env.h"
#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CBinaryReader : public CStream {

    __extends(CStream);

    public:

        CBinaryReader(IStream *baseStream);

        int8_t ReadInt8() const;

        uint8_t ReadUInt8() const;

        int16_t ReadInt16LE() const;

        int16_t ReadInt16BE() const;

        uint16_t ReadUInt16LE() const;

        uint16_t ReadUInt16BE() const;

        int32_t ReadInt32LE() const;

        int32_t ReadInt32BE() const;

        uint32_t ReadUInt32LE() const;

        uint32_t ReadUInt32BE() const;

        int64_t ReadInt64LE() const;

        int64_t ReadInt64BE() const;

        uint64_t ReadUInt64LE() const;

        uint64_t ReadUInt64BE() const;

        float ReadSingleLE() const;

        float ReadSingleBE() const;

        double ReadDoubleLE() const;

        double ReadDoubleBE() const;

        int8_t PeekInt8();

        uint8_t PeekUInt8();

        int16_t PeekInt16LE();

        int16_t PeekInt16BE();

        uint16_t PeekUInt16LE();

        uint16_t PeekUInt16BE();

        int32_t PeekInt32LE();

        int32_t PeekInt32BE();

        uint32_t PeekUInt32LE();

        uint32_t PeekUInt32BE();

        int64_t PeekInt64LE();

        int64_t PeekInt64BE();

        uint64_t PeekUInt64LE();

        uint64_t PeekUInt64BE();

        float PeekSingleLE();

        float PeekSingleBE();

        double PeekDoubleLE();

        double PeekDoubleBE();

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

        CBinaryReader() = delete;

        CBinaryReader(const CBinaryReader &) = delete;

        IStream *_baseStream;

    };

CGSS_NS_END
