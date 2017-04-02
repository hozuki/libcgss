#pragma once

#include "../../cgss_env.h"
#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CBinaryReader : public CStream {

    __extends(CStream, CBinaryReader);

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

        static int8_t ReadInt8(IStream *stream);

        static uint8_t ReadUInt8(IStream *stream);

        static int16_t ReadInt16LE(IStream *stream);

        static int16_t ReadInt16BE(IStream *stream);

        static uint16_t ReadUInt16LE(IStream *stream);

        static uint16_t ReadUInt16BE(IStream *stream);

        static int32_t ReadInt32LE(IStream *stream);

        static int32_t ReadInt32BE(IStream *stream);

        static uint32_t ReadUInt32LE(IStream *stream);

        static uint32_t ReadUInt32BE(IStream *stream);

        static int64_t ReadInt64LE(IStream *stream);

        static int64_t ReadInt64BE(IStream *stream);

        static uint64_t ReadUInt64LE(IStream *stream);

        static uint64_t ReadUInt64BE(IStream *stream);

        static float ReadSingleLE(IStream *stream);

        static float ReadSingleBE(IStream *stream);

        static double ReadDoubleLE(IStream *stream);

        static double ReadDoubleBE(IStream *stream);

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

        static int8_t PeekInt8(IStream *stream);

        static uint8_t PeekUInt8(IStream *stream);

        static int16_t PeekInt16LE(IStream *stream);

        static int16_t PeekInt16BE(IStream *stream);

        static uint16_t PeekUInt16LE(IStream *stream);

        static uint16_t PeekUInt16BE(IStream *stream);

        static int32_t PeekInt32LE(IStream *stream);

        static int32_t PeekInt32BE(IStream *stream);

        static uint32_t PeekUInt32LE(IStream *stream);

        static uint32_t PeekUInt32BE(IStream *stream);

        static int64_t PeekInt64LE(IStream *stream);

        static int64_t PeekInt64BE(IStream *stream);

        static uint64_t PeekUInt64LE(IStream *stream);

        static uint64_t PeekUInt64BE(IStream *stream);

        static float PeekSingleLE(IStream *stream);

        static float PeekSingleBE(IStream *stream);

        static double PeekDoubleLE(IStream *stream);

        static double PeekDoubleBE(IStream *stream);

        static int8_t PeekInt8(IStream *stream, uint64_t offset);

        static uint8_t PeekUInt8(IStream *stream, uint64_t offset);

        static int16_t PeekInt16LE(IStream *stream, uint64_t offset);

        static int16_t PeekInt16BE(IStream *stream, uint64_t offset);

        static uint16_t PeekUInt16LE(IStream *stream, uint64_t offset);

        static uint16_t PeekUInt16BE(IStream *stream, uint64_t offset);

        static int32_t PeekInt32LE(IStream *stream, uint64_t offset);

        static int32_t PeekInt32BE(IStream *stream, uint64_t offset);

        static uint32_t PeekUInt32LE(IStream *stream, uint64_t offset);

        static uint32_t PeekUInt32BE(IStream *stream, uint64_t offset);

        static int64_t PeekInt64LE(IStream *stream, uint64_t offset);

        static int64_t PeekInt64BE(IStream *stream, uint64_t offset);

        static uint64_t PeekUInt64LE(IStream *stream, uint64_t offset);

        static uint64_t PeekUInt64BE(IStream *stream, uint64_t offset);

        static float PeekSingleLE(IStream *stream, uint64_t offset);

        static float PeekSingleBE(IStream *stream, uint64_t offset);

        static double PeekDoubleLE(IStream *stream, uint64_t offset);

        static double PeekDoubleBE(IStream *stream, uint64_t offset);

        static uint32_t PeekBytes(IStream *stream, uint8_t *buffer, uint32_t bufferSize, size_t offset, uint32_t count);

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
