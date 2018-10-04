#pragma once

#include "../../cgss_env.h"
#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CBinaryReader : public CStream {

    __extends(CStream, CBinaryReader);

    public:

        explicit CBinaryReader(IStream *baseStream);

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

        int8_t ReadInt8(uint64_t offset) const;

        uint8_t ReadUInt8(uint64_t offset) const;

        int16_t ReadInt16LE(uint64_t offset) const;

        int16_t ReadInt16BE(uint64_t offset) const;

        uint16_t ReadUInt16LE(uint64_t offset) const;

        uint16_t ReadUInt16BE(uint64_t offset) const;

        int32_t ReadInt32LE(uint64_t offset) const;

        int32_t ReadInt32BE(uint64_t offset) const;

        uint32_t ReadUInt32LE(uint64_t offset) const;

        uint32_t ReadUInt32BE(uint64_t offset) const;

        int64_t ReadInt64LE(uint64_t offset) const;

        int64_t ReadInt64BE(uint64_t offset) const;

        uint64_t ReadUInt64LE(uint64_t offset) const;

        uint64_t ReadUInt64BE(uint64_t offset) const;

        float ReadSingleLE(uint64_t offset) const;

        float ReadSingleBE(uint64_t offset) const;

        double ReadDoubleLE(uint64_t offset) const;

        double ReadDoubleBE(uint64_t offset) const;

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

        static int8_t ReadInt8(IStream *stream, uint64_t offset);

        static uint8_t ReadUInt8(IStream *stream, uint64_t offset);

        static int16_t ReadInt16LE(IStream *stream, uint64_t offset);

        static int16_t ReadInt16BE(IStream *stream, uint64_t offset);

        static uint16_t ReadUInt16LE(IStream *stream, uint64_t offset);

        static uint16_t ReadUInt16BE(IStream *stream, uint64_t offset);

        static int32_t ReadInt32LE(IStream *stream, uint64_t offset);

        static int32_t ReadInt32BE(IStream *stream, uint64_t offset);

        static uint32_t ReadUInt32LE(IStream *stream, uint64_t offset);

        static uint32_t ReadUInt32BE(IStream *stream, uint64_t offset);

        static int64_t ReadInt64LE(IStream *stream, uint64_t offset);

        static int64_t ReadInt64BE(IStream *stream, uint64_t offset);

        static uint64_t ReadUInt64LE(IStream *stream, uint64_t offset);

        static uint64_t ReadUInt64BE(IStream *stream, uint64_t offset);

        static float ReadSingleLE(IStream *stream, uint64_t offset);

        static float ReadSingleBE(IStream *stream, uint64_t offset);

        static double ReadDoubleLE(IStream *stream, uint64_t offset);

        static double ReadDoubleBE(IStream *stream, uint64_t offset);

        int8_t PeekInt8() const;

        uint8_t PeekUInt8() const;

        int16_t PeekInt16LE() const;

        int16_t PeekInt16BE() const;

        uint16_t PeekUInt16LE() const;

        uint16_t PeekUInt16BE() const;

        int32_t PeekInt32LE() const;

        int32_t PeekInt32BE() const;

        uint32_t PeekUInt32LE() const;

        uint32_t PeekUInt32BE() const;

        int64_t PeekInt64LE() const;

        int64_t PeekInt64BE() const;

        uint64_t PeekUInt64LE() const;

        uint64_t PeekUInt64BE() const;

        float PeekSingleLE() const;

        float PeekSingleBE() const;

        double PeekDoubleLE() const;

        double PeekDoubleBE() const;

        int8_t PeekInt8(uint64_t offset) const;

        uint8_t PeekUInt8(uint64_t offset) const;

        int16_t PeekInt16LE(uint64_t offset) const;

        int16_t PeekInt16BE(uint64_t offset) const;

        uint16_t PeekUInt16LE(uint64_t offset) const;

        uint16_t PeekUInt16BE(uint64_t offset) const;

        int32_t PeekInt32LE(uint64_t offset) const;

        int32_t PeekInt32BE(uint64_t offset) const;

        uint32_t PeekUInt32LE(uint64_t offset) const;

        uint32_t PeekUInt32BE(uint64_t offset) const;

        int64_t PeekInt64LE(uint64_t offset) const;

        int64_t PeekInt64BE(uint64_t offset) const;

        uint64_t PeekUInt64LE(uint64_t offset) const;

        uint64_t PeekUInt64BE(uint64_t offset) const;

        float PeekSingleLE(uint64_t offset) const;

        float PeekSingleBE(uint64_t offset) const;

        double PeekDoubleLE(uint64_t offset) const;

        double PeekDoubleBE(uint64_t offset) const;

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

        uint32_t Peek(void *buffer, uint32_t bufferSize, size_t bufferOffset, uint32_t count);

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

    private:

        CBinaryReader() = delete;

        CBinaryReader(const CBinaryReader &) = delete;

        IStream *_baseStream;

    };

CGSS_NS_END
