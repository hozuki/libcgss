#include "CBinaryWriter.h"
#include "../../common/quick_utils.h"
#include "../exceptions/CInvalidOperationException.h"

// http://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
enum {
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_PDP_ENDIAN = 0x01000302ul
};

static const union {
    uint8_t bytes[4];
    uint32_t value;
} _o32_host_order = {
    {0, 1, 2, 3}
};

#define O32_HOST_ORDER (_o32_host_order.value)

#define WRITE_XINT(bit, hostEndian) \
    if (O32_HOST_ORDER != (hostEndian)) { \
        v = bswap(v); \
    } \
    uint8_t *buffer = (uint8_t *)&v; \
    static const auto bufferSize = (bit) / 8; \
    auto written = _baseStream->Write(buffer, bufferSize, 0, bufferSize); \
    return written

CGSS_NS_BEGIN

    CBinaryWriter::CBinaryWriter(IStream *baseStream)
        : _baseStream(baseStream) {
    }

    uint32_t CBinaryWriter::WriteInt8(int8_t v) {
        uint8_t u = *(uint8_t *)&v;
        return _baseStream->WriteByte(u);
    }

    uint32_t CBinaryWriter::WriteUInt8(uint8_t v) {
        return _baseStream->WriteByte(v);
    }

    uint32_t CBinaryWriter::WriteInt16LE(int16_t v) {
        WRITE_XINT(16, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteInt16BE(int16_t v) {
        WRITE_XINT(16, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteUInt16LE(uint16_t v) {
        WRITE_XINT(16, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteUInt16BE(uint16_t v) {
        WRITE_XINT(16, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteInt32LE(int32_t v) {
        WRITE_XINT(32, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteInt32BE(int32_t v) {
        WRITE_XINT(32, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteUInt32LE(uint32_t v) {
        WRITE_XINT(32, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteUInt32BE(uint32_t v) {
        WRITE_XINT(32, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteInt64LE(int64_t v) {
        WRITE_XINT(64, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteInt64BE(int64_t v) {
        WRITE_XINT(64, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteUInt64LE(uint64_t v) {
        WRITE_XINT(64, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteUInt64BE(uint64_t v) {
        WRITE_XINT(64, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryWriter::WriteSingleLE(float v) {
        int32_t i = *(int32_t *)&v;
        return WriteInt32LE(i);
    }

    uint32_t CBinaryWriter::WriteSingleBE(float v) {
        int32_t i = *(int32_t *)&v;
        return WriteInt32BE(i);
    }

    uint32_t CBinaryWriter::WriteDoubleLE(double v) {
        int64_t i = *(int64_t *)&v;
        return WriteInt64LE(i);
    }

    uint32_t CBinaryWriter::WriteDoubleBE(double v) {
        int64_t i = *(int64_t *)&v;
        return WriteInt64BE(i);
    }

    uint32_t CBinaryWriter::Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        throw CInvalidOperationException("CBinaryWriter::Read");
    }

    uint32_t CBinaryWriter::Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        return _baseStream->Write(buffer, bufferSize, offset, count);
    }

    bool_t CBinaryWriter::IsWritable() const {
        return _baseStream->IsWritable();
    }

    bool_t CBinaryWriter::IsReadable() const {
        return FALSE;
    }

    bool_t CBinaryWriter::IsSeekable() const {
        return _baseStream->IsSeekable();
    }

    uint64_t CBinaryWriter::GetPosition() {
        return _baseStream->GetPosition();
    }

    void CBinaryWriter::SetPosition(uint64_t value) {
        _baseStream->SetPosition(value);
    }

    uint64_t CBinaryWriter::GetLength() {
        return _baseStream->GetLength();
    }

    void CBinaryWriter::SetLength(uint64_t value) {
        _baseStream->SetLength(value);
    }

    void CBinaryWriter::Flush() {
        _baseStream->Flush();
    }

CGSS_NS_END
