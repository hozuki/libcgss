#include "CBinaryReader.h"
#include "../exceptions/CException.h"
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

CGSS_NS_BEGIN

#define ENSURE_READ(n)  \
    static const auto shouldRead = (n); \
    uint8_t buffer[shouldRead]; \
    auto read = _baseStream->Read(buffer, shouldRead, 0, shouldRead); \
    do { \
        if (read < shouldRead) { \
            throw CException(CGSS_OP_BUFFER_TOO_SMALL); \
        } \
    } while (0)

#define READ_INT(bit, hostEndian) \
    ENSURE_READ((bit) / 8); \
    int##bit##_t ret = *(int##bit##_t *)buffer; \
    if (O32_HOST_ORDER != (hostEndian)) { \
        ret = bswap(ret); \
    } \
    return ret

#define READ_UINT(bit, hostEndian) \
    ENSURE_READ((bit) / 8); \
    uint##bit##_t ret = *(uint##bit##_t *)buffer; \
    if (O32_HOST_ORDER != (hostEndian)) { \
        ret = bswap(ret); \
    } \
    return ret

    CBinaryReader::CBinaryReader(CStream *baseStream)
        : _baseStream(baseStream) {
    }

    int8_t CBinaryReader::ReadInt8() const {
        uint8_t u = _baseStream->ReadByte();
        int8_t i = *(int8_t *)&u;
        return i;
    }

    uint8_t CBinaryReader::ReadUInt8() const {
        return _baseStream->ReadByte();
    }

    int16_t CBinaryReader::ReadInt16LE() const {
        READ_INT(16, O32_LITTLE_ENDIAN);
    }

    int16_t CBinaryReader::ReadInt16BE() const {
        READ_INT(16, O32_BIG_ENDIAN);
    }

    uint16_t CBinaryReader::ReadUInt16LE() const {
        READ_UINT(16, O32_LITTLE_ENDIAN);
    }

    uint16_t CBinaryReader::ReadUInt16BE() const {
        READ_UINT(16, O32_BIG_ENDIAN);
    }

    int32_t CBinaryReader::ReadInt32LE() const {
        READ_INT(32, O32_LITTLE_ENDIAN);
    }

    int32_t CBinaryReader::ReadInt32BE() const {
        READ_INT(32, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryReader::ReadUInt32LE() const {
        READ_UINT(32, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryReader::ReadUInt32BE() const {
        READ_UINT(32, O32_BIG_ENDIAN);
    }

    int64_t CBinaryReader::ReadInt64LE() const {
        READ_INT(64, O32_LITTLE_ENDIAN);
    }

    int64_t CBinaryReader::ReadInt64BE() const {
        READ_INT(64, O32_BIG_ENDIAN);
    }

    uint64_t CBinaryReader::ReadUInt64LE() const {
        READ_UINT(64, O32_LITTLE_ENDIAN);
    }

    uint64_t CBinaryReader::ReadUInt64BE() const {
        READ_UINT(64, O32_BIG_ENDIAN);
    }

    float CBinaryReader::ReadSingleLE() const {
        auto i = ReadInt32LE();
        auto f = *(float *)&i;
        return f;
    }

    float CBinaryReader::ReadSingleBE() const {
        auto i = ReadInt32BE();
        auto f = *(float *)&i;
        return f;
    }

    double CBinaryReader::ReadDoubleLE() const {
        auto i = ReadInt64LE();
        auto f = *(double *)&i;
        return f;
    }

    double CBinaryReader::ReadDoubleBE() const {
        auto i = ReadInt64BE();
        auto f = *(double *)&i;
        return f;
    }

#define PEEK_WRAP(funcName) \
    auto position = GetPosition(); \
    auto value = funcName; \
    SetPosition(position); \
    return value

    int8_t CBinaryReader::PeekInt8() {
        PEEK_WRAP(ReadInt8());
    }

    uint8_t CBinaryReader::PeekUInt8() {
        PEEK_WRAP(ReadUInt8());
    }

    int16_t CBinaryReader::PeekInt16LE() {
        PEEK_WRAP(ReadInt16LE());
    }

    int16_t CBinaryReader::PeekInt16BE() {
        PEEK_WRAP(ReadInt16BE());
    }

    uint16_t CBinaryReader::PeekUInt16LE() {
        PEEK_WRAP(ReadUInt16LE());
    }

    uint16_t CBinaryReader::PeekUInt16BE() {
        PEEK_WRAP(ReadUInt16BE());
    }

    int32_t CBinaryReader::PeekInt32LE() {
        PEEK_WRAP(ReadInt32LE());
    }

    int32_t CBinaryReader::PeekInt32BE() {
        PEEK_WRAP(ReadInt32BE());
    }

    uint32_t CBinaryReader::PeekUInt32LE() {
        PEEK_WRAP(ReadUInt32LE());
    }

    uint32_t CBinaryReader::PeekUInt32BE() {
        PEEK_WRAP(ReadUInt32BE());
    }

    int64_t CBinaryReader::PeekInt64LE() {
        PEEK_WRAP(ReadInt64LE());
    }

    int64_t CBinaryReader::PeekInt64BE() {
        PEEK_WRAP(ReadInt64BE());
    }

    uint64_t CBinaryReader::PeekUInt64LE() {
        PEEK_WRAP(ReadUInt64LE());
    }

    uint64_t CBinaryReader::PeekUInt64BE() {
        PEEK_WRAP(ReadUInt64BE());
    }

    float CBinaryReader::PeekSingleLE() {
        PEEK_WRAP(ReadSingleLE());
    }

    float CBinaryReader::PeekSingleBE() {
        PEEK_WRAP(ReadSingleBE());
    }

    double CBinaryReader::PeekDoubleLE() {
        PEEK_WRAP(ReadDoubleLE());
    }

    double CBinaryReader::PeekDoubleBE() {
        PEEK_WRAP(ReadDoubleBE());
    }

    uint32_t CBinaryReader::Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        return _baseStream->Read(buffer, bufferSize, offset, count);
    }

    uint32_t CBinaryReader::Write(const void *buffer, uint32_t bufferSize, uint32_t offset, uint32_t count) {
        throw CInvalidOperationException("CBinaryReader::Write");
    }

    bool_t CBinaryReader::IsReadable() const {
        return _baseStream->IsReadable();
    }

    bool_t CBinaryReader::IsWritable() const {
        return FALSE;
    }

    bool_t CBinaryReader::IsSeekable() const {
        return _baseStream->IsSeekable();
    }

    uint64_t CBinaryReader::GetPosition() {
        return _baseStream->GetPosition();
    }

    void CBinaryReader::SetPosition(uint64_t value) {
        _baseStream->SetPosition(value);
    }

    uint64_t CBinaryReader::GetLength() {
        return _baseStream->GetLength();
    }

    void CBinaryReader::SetLength(uint64_t value) {
        _baseStream->SetLength(value);
    }

    void CBinaryReader::Flush() {
        throw CInvalidOperationException("CBinaryReader::Flush");
    }

CGSS_NS_END
