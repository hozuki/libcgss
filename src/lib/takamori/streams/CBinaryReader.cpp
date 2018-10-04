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

    CBinaryReader::CBinaryReader(IStream *baseStream)
        : _baseStream(baseStream) {
    }

    int8_t CBinaryReader::ReadInt8(IStream *stream) {
        uint8_t u = stream->ReadByte();
        int8_t i = *(int8_t *)&u;
        return i;
    }

    uint8_t CBinaryReader::ReadUInt8(IStream *stream) {
        return stream->ReadByte();
    }

#define ENSURE_READ(n)  \
    static const auto shouldRead = (n); \
    uint8_t buffer[shouldRead]; \
    auto read = stream->Read(buffer, shouldRead, 0, shouldRead); \
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

    int16_t CBinaryReader::ReadInt16LE(IStream *stream) {
        READ_INT(16, O32_LITTLE_ENDIAN);
    }

    int16_t CBinaryReader::ReadInt16BE(IStream *stream) {
        READ_INT(16, O32_BIG_ENDIAN);
    }

    uint16_t CBinaryReader::ReadUInt16LE(IStream *stream) {
        READ_UINT(16, O32_LITTLE_ENDIAN);
    }

    uint16_t CBinaryReader::ReadUInt16BE(IStream *stream) {
        READ_UINT(16, O32_BIG_ENDIAN);
    }

    int32_t CBinaryReader::ReadInt32LE(IStream *stream) {
        READ_INT(32, O32_LITTLE_ENDIAN);
    }

    int32_t CBinaryReader::ReadInt32BE(IStream *stream) {
        READ_INT(32, O32_BIG_ENDIAN);
    }

    uint32_t CBinaryReader::ReadUInt32LE(IStream *stream) {
        READ_UINT(32, O32_LITTLE_ENDIAN);
    }

    uint32_t CBinaryReader::ReadUInt32BE(IStream *stream) {
        READ_UINT(32, O32_BIG_ENDIAN);
    }

    int64_t CBinaryReader::ReadInt64LE(IStream *stream) {
        READ_INT(64, O32_LITTLE_ENDIAN);
    }

    int64_t CBinaryReader::ReadInt64BE(IStream *stream) {
        READ_INT(64, O32_BIG_ENDIAN);
    }

    uint64_t CBinaryReader::ReadUInt64LE(IStream *stream) {
        READ_UINT(64, O32_LITTLE_ENDIAN);
    }

    uint64_t CBinaryReader::ReadUInt64BE(IStream *stream) {
        READ_UINT(64, O32_BIG_ENDIAN);
    }

    float CBinaryReader::ReadSingleLE(IStream *stream) {
        auto i = ReadInt32LE(stream);
        auto f = *(float *)&i;
        return f;
    }

    float CBinaryReader::ReadSingleBE(IStream *stream) {
        auto i = ReadInt32BE(stream);
        auto f = *(float *)&i;
        return f;
    }

    double CBinaryReader::ReadDoubleLE(IStream *stream) {
        auto i = ReadInt64LE(stream);
        auto f = *(double *)&i;
        return f;
    }

    double CBinaryReader::ReadDoubleBE(IStream *stream) {
        auto i = ReadInt64BE(stream);
        auto f = *(double *)&i;
        return f;
    }

#define READ_U_FUNC_O(bit, suffix) \
    uint##bit##_t CBinaryReader::ReadUInt##bit##suffix(IStream *stream, uint64_t offset) { \
        auto position = stream->GetPosition(); \
        stream->Seek(offset, StreamSeekOrigin::Begin); \
        auto value = ReadUInt##bit##suffix(stream); \
        stream->SetPosition(position); \
        return value; \
    }

#define READ_S_FUNC_O(bit, suffix) \
    int##bit##_t CBinaryReader::ReadInt##bit##suffix(IStream *stream, uint64_t offset) { \
        auto position = stream->GetPosition(); \
        stream->Seek(offset, StreamSeekOrigin::Begin); \
        auto value = ReadInt##bit##suffix(stream); \
        stream->SetPosition(position); \
        return value; \
    }

    READ_S_FUNC_O(8,)

    READ_U_FUNC_O(8,)

    READ_S_FUNC_O(16, LE)

    READ_U_FUNC_O(16, LE)

    READ_S_FUNC_O(32, LE)

    READ_U_FUNC_O(32, LE)

    READ_S_FUNC_O(64, LE)

    READ_U_FUNC_O(64, LE)

    READ_S_FUNC_O(16, BE)

    READ_U_FUNC_O(16, BE)

    READ_S_FUNC_O(32, BE)

    READ_U_FUNC_O(32, BE)

    READ_S_FUNC_O(64, BE)

    READ_U_FUNC_O(64, BE)

#define READ_R_FUNC(type, Cap, suffix) \
    type CBinaryReader::Read##Cap##suffix(IStream *stream, uint64_t offset) { \
        auto position = stream->GetPosition(); \
        stream->Seek(offset, StreamSeekOrigin::Begin); \
        auto value = Read##Cap##suffix(stream); \
        stream->SetPosition(position); \
        return value; \
    }

    READ_R_FUNC(float, Single, LE)

    READ_R_FUNC(float, Single, BE)

    READ_R_FUNC(double, Double, LE)

    READ_R_FUNC(double, Double, BE)

#define PEEK_U_FUNC(bit, suffix) \
    uint##bit##_t CBinaryReader::PeekUInt##bit##suffix(IStream *stream) { \
        auto position = stream->GetPosition(); \
        auto value = ReadUInt##bit##suffix(stream); \
        stream->SetPosition(position); \
        return value; \
    }

#define PEEK_S_FUNC(bit, suffix) \
    int##bit##_t CBinaryReader::PeekInt##bit##suffix(IStream *stream) { \
        auto position = stream->GetPosition(); \
        auto value = ReadInt##bit##suffix(stream); \
        stream->SetPosition(position); \
        return value; \
    }

    PEEK_S_FUNC(8,)

    PEEK_U_FUNC(8,)

    PEEK_S_FUNC(16, LE)

    PEEK_U_FUNC(16, LE)

    PEEK_S_FUNC(32, LE)

    PEEK_U_FUNC(32, LE)

    PEEK_S_FUNC(64, LE)

    PEEK_U_FUNC(64, LE)

    PEEK_S_FUNC(16, BE)

    PEEK_U_FUNC(16, BE)

    PEEK_S_FUNC(32, BE)

    PEEK_U_FUNC(32, BE)

    PEEK_S_FUNC(64, BE)

    PEEK_U_FUNC(64, BE)

#define PEEK_R_FUNC(type, Cap, suffix) \
    type CBinaryReader::Peek##Cap##suffix(IStream *stream) { \
        auto position = stream->GetPosition(); \
        auto value = Read##Cap##suffix(stream); \
        stream->SetPosition(position); \
        return value; \
    }

    PEEK_R_FUNC(float, Single, LE)

    PEEK_R_FUNC(float, Single, BE)

    PEEK_R_FUNC(double, Double, LE)

    PEEK_R_FUNC(double, Double, BE)

#define READ_INSTANCE_WRAP_S(bit, suffix) \
    int##bit##_t CBinaryReader::ReadInt##bit##suffix() const { \
        return ReadInt##bit##suffix(_baseStream); \
    }

#define READ_INSTANCE_WRAP_U(bit, suffix) \
    uint##bit##_t CBinaryReader::ReadUInt##bit##suffix() const { \
        return ReadUInt##bit##suffix(_baseStream); \
    }

    READ_INSTANCE_WRAP_S(8,)

    READ_INSTANCE_WRAP_S(16, LE)

    READ_INSTANCE_WRAP_S(16, BE)

    READ_INSTANCE_WRAP_S(32, LE)

    READ_INSTANCE_WRAP_S(32, BE)

    READ_INSTANCE_WRAP_S(64, LE)

    READ_INSTANCE_WRAP_S(64, BE)

    READ_INSTANCE_WRAP_U(8,)

    READ_INSTANCE_WRAP_U(16, LE)

    READ_INSTANCE_WRAP_U(16, BE)

    READ_INSTANCE_WRAP_U(32, LE)

    READ_INSTANCE_WRAP_U(32, BE)

    READ_INSTANCE_WRAP_U(64, LE)

    READ_INSTANCE_WRAP_U(64, BE)

#define READ_INSTANCE_WRAP_S_O(bit, suffix) \
    int##bit##_t CBinaryReader::ReadInt##bit##suffix(uint64_t offset) const { \
        return ReadInt##bit##suffix(_baseStream, offset); \
    }

#define READ_INSTANCE_WRAP_U_O(bit, suffix) \
    uint##bit##_t CBinaryReader::ReadUInt##bit##suffix(uint64_t offset) const { \
        return ReadUInt##bit##suffix(_baseStream, offset); \
    }

    READ_INSTANCE_WRAP_S_O(8,)

    READ_INSTANCE_WRAP_S_O(16, LE)

    READ_INSTANCE_WRAP_S_O(16, BE)

    READ_INSTANCE_WRAP_S_O(32, LE)

    READ_INSTANCE_WRAP_S_O(32, BE)

    READ_INSTANCE_WRAP_S_O(64, LE)

    READ_INSTANCE_WRAP_S_O(64, BE)

    READ_INSTANCE_WRAP_U_O(8,)

    READ_INSTANCE_WRAP_U_O(16, LE)

    READ_INSTANCE_WRAP_U_O(16, BE)

    READ_INSTANCE_WRAP_U_O(32, LE)

    READ_INSTANCE_WRAP_U_O(32, BE)

    READ_INSTANCE_WRAP_U_O(64, LE)

    READ_INSTANCE_WRAP_U_O(64, BE)

#define PEEK_INSTANCE_WRAP_S(bit, suffix) \
    int##bit##_t CBinaryReader::PeekInt##bit##suffix() const { \
        return PeekInt##bit##suffix(_baseStream); \
    }

#define PEEK_INSTANCE_WRAP_U(bit, suffix) \
    uint##bit##_t CBinaryReader::PeekUInt##bit##suffix() const { \
        return PeekUInt##bit##suffix(_baseStream); \
    }

    PEEK_INSTANCE_WRAP_S(8,)

    PEEK_INSTANCE_WRAP_S(16, LE)

    PEEK_INSTANCE_WRAP_S(16, BE)

    PEEK_INSTANCE_WRAP_S(32, LE)

    PEEK_INSTANCE_WRAP_S(32, BE)

    PEEK_INSTANCE_WRAP_S(64, LE)

    PEEK_INSTANCE_WRAP_S(64, BE)

    PEEK_INSTANCE_WRAP_U(8,)

    PEEK_INSTANCE_WRAP_U(16, LE)

    PEEK_INSTANCE_WRAP_U(16, BE)

    PEEK_INSTANCE_WRAP_U(32, LE)

    PEEK_INSTANCE_WRAP_U(32, BE)

    PEEK_INSTANCE_WRAP_U(64, LE)

    PEEK_INSTANCE_WRAP_U(64, BE)

#define READ_INSTANCE_WRAP_R(type, Cap, suffix) \
    type CBinaryReader::Read##Cap##suffix() const { \
        return Read##Cap##suffix(_baseStream); \
    }

    READ_INSTANCE_WRAP_R(float, Single, LE)

    READ_INSTANCE_WRAP_R(float, Single, BE)

    READ_INSTANCE_WRAP_R(double, Double, LE)

    READ_INSTANCE_WRAP_R(double, Double, BE)

#define READ_INSTANCE_WRAP_R_O(type, Cap, suffix) \
    type CBinaryReader::Read##Cap##suffix(uint64_t offset) const { \
        return Read##Cap##suffix(_baseStream, offset); \
    }

    READ_INSTANCE_WRAP_R_O(float, Single, LE)

    READ_INSTANCE_WRAP_R_O(float, Single, BE)

    READ_INSTANCE_WRAP_R_O(double, Double, LE)

    READ_INSTANCE_WRAP_R_O(double, Double, BE)

#define PEEK_INSTANCE_WRAP_R(type, Cap, suffix) \
    type CBinaryReader::Peek##Cap##suffix() const { \
        return Peek##Cap##suffix(_baseStream); \
    }

    PEEK_INSTANCE_WRAP_R(float, Single, LE)

    PEEK_INSTANCE_WRAP_R(float, Single, BE)

    PEEK_INSTANCE_WRAP_R(double, Double, LE)

    PEEK_INSTANCE_WRAP_R(double, Double, BE)

#define PEEK_INSTANCE_WRAP_R_O(type, Cap, suffix) \
    type CBinaryReader::Peek##Cap##suffix(uint64_t offset) const { \
        return Peek##Cap##suffix(_baseStream); \
    }

    PEEK_INSTANCE_WRAP_R_O(float, Single, LE)

    PEEK_INSTANCE_WRAP_R_O(float, Single, BE)

    PEEK_INSTANCE_WRAP_R_O(double, Double, LE)

    PEEK_INSTANCE_WRAP_R_O(double, Double, BE)

#define PEEK_WRAP_S_O(bit, suffix) \
    int##bit##_t CBinaryReader::PeekInt##bit##suffix(IStream *stream, uint64_t offset) { \
        auto position = stream->GetPosition(); \
        if (stream->GetPosition() != offset) { \
            stream->Seek(offset, StreamSeekOrigin::Begin); \
        } \
        auto value = ReadInt##bit##suffix(stream); \
        stream->Seek(position, StreamSeekOrigin::Begin); \
        return value; \
    }

#define PEEK_WRAP_U_O(bit, suffix) \
    uint##bit##_t CBinaryReader::PeekUInt##bit##suffix(IStream *stream, uint64_t offset) { \
        auto position = stream->GetPosition(); \
        if (stream->GetPosition() != offset) { \
            stream->Seek(offset, StreamSeekOrigin::Begin); \
        } \
        auto value = ReadUInt##bit##suffix(stream); \
        stream->Seek(position, StreamSeekOrigin::Begin); \
        return value; \
    }

    PEEK_WRAP_S_O(8,)

    PEEK_WRAP_U_O(8,)

    PEEK_WRAP_S_O(16, LE)

    PEEK_WRAP_U_O(16, LE)

    PEEK_WRAP_S_O(16, BE)

    PEEK_WRAP_U_O(16, BE)

    PEEK_WRAP_S_O(32, LE)

    PEEK_WRAP_U_O(32, LE)

    PEEK_WRAP_S_O(32, BE)

    PEEK_WRAP_U_O(32, BE)

    PEEK_WRAP_S_O(64, LE)

    PEEK_WRAP_U_O(64, LE)

    PEEK_WRAP_S_O(64, BE)

    PEEK_WRAP_U_O(64, BE)

#define PEEK_INSTANCE_WRAP_S_O(bit, suffix) \
    int##bit##_t CBinaryReader::PeekInt##bit##suffix(uint64_t offset) const { \
        return PeekInt##bit##suffix(_baseStream, offset); \
    }

#define PEEK_INSTANCE_WRAP_U_O(bit, suffix) \
    uint##bit##_t CBinaryReader::PeekUInt##bit##suffix(uint64_t offset) const { \
        return PeekUInt##bit##suffix(_baseStream, offset); \
    }

    PEEK_INSTANCE_WRAP_S_O(8,)

    PEEK_INSTANCE_WRAP_S_O(16, LE)

    PEEK_INSTANCE_WRAP_S_O(16, BE)

    PEEK_INSTANCE_WRAP_S_O(32, LE)

    PEEK_INSTANCE_WRAP_S_O(32, BE)

    PEEK_INSTANCE_WRAP_S_O(64, LE)

    PEEK_INSTANCE_WRAP_S_O(64, BE)

    PEEK_INSTANCE_WRAP_U_O(8,)

    PEEK_INSTANCE_WRAP_U_O(16, LE)

    PEEK_INSTANCE_WRAP_U_O(16, BE)

    PEEK_INSTANCE_WRAP_U_O(32, LE)

    PEEK_INSTANCE_WRAP_U_O(32, BE)

    PEEK_INSTANCE_WRAP_U_O(64, LE)

    PEEK_INSTANCE_WRAP_U_O(64, BE)

#define PEEK_WRAP_R_O(type, Cap, suffix) \
    type CBinaryReader::Peek##Cap##suffix(IStream *stream, uint64_t offset) { \
        auto position = stream->GetPosition(); \
        if (stream->GetPosition() != offset) { \
            stream->Seek(offset, StreamSeekOrigin::Begin); \
        } \
        auto value = Read##Cap##suffix(stream); \
        stream->Seek(position, StreamSeekOrigin::Begin); \
        return value; \
    }

    PEEK_WRAP_R_O(float, Single, LE)

    PEEK_WRAP_R_O(float, Single, BE)

    PEEK_WRAP_R_O(double, Double, LE)

    PEEK_WRAP_R_O(double, Double, BE)

    uint32_t CBinaryReader::Peek(void *buffer, uint32_t bufferSize, size_t bufferOffset, uint32_t count) {
        return PeekBytes(_baseStream, static_cast<uint8_t *>(buffer), bufferSize, bufferOffset, count);
    }

    uint32_t CBinaryReader::Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        return _baseStream->Read(buffer, bufferSize, offset, count);
    }

    uint32_t CBinaryReader::Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
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

    uint32_t CBinaryReader::PeekBytes(IStream *stream, uint8_t *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        const auto position = stream->GetPosition();
        const auto v = stream->Read(buffer, bufferSize, offset, count);
        stream->Seek(position, StreamSeekOrigin::Begin);
        return v;
    }

CGSS_NS_END
