#include "CUtfReader.h"
#include "../takamori/streams/CBinaryReader.h"
#include "../takamori/CBitConverter.h"

CGSS_NS_BEGIN

    CUtfReader::CUtfReader()
        : _encrypted(FALSE), _seed(0), _increment(0) {
        _currentXor = 0;
        _currentUtfOffset = 0;
        _currentStringXor = 0;
        _currentUtfStringOffset = 0;
    }

    CUtfReader::CUtfReader(uint8_t seed, uint8_t increment)
        : _encrypted(TRUE), _seed(seed), _increment(increment) {
        _currentXor = 0;
        _currentUtfOffset = 0;
        _currentStringXor = 0;
        _currentUtfStringOffset = 0;
    }

    bool_t CUtfReader::IsEncrypted() const {
        return _encrypted;
    }

    void CUtfReader::PeekBytes(IStream *stream, uint8_t *buffer, uint64_t streamOffset, uint32_t size, uint64_t utfOffset) {
        PeekBytes(stream, buffer, 0, streamOffset, size, utfOffset);
    }

    void CUtfReader::PeekBytes(IStream *stream, uint8_t *buffer, uint64_t bufferOffset, uint64_t streamOffset, uint32_t size, uint64_t utfOffset) {
        stream->Seek(streamOffset + utfOffset, StreamSeekOrigin::Begin);

        CBinaryReader::PeekBytes(stream, buffer, size, static_cast<size_t>(bufferOffset), size);

        if (!IsEncrypted()) {
            return;
        }

        if (utfOffset < _currentUtfOffset) {
            _currentUtfOffset = 0;
        }

        if (_currentUtfOffset == 0) {
            _currentXor = _seed;
        }

        for (auto j = _currentUtfOffset; j < utfOffset; ++j) {
            if (j > 0) {
                _currentXor *= _increment;
            }
            ++_currentUtfOffset;
        }

        for (uint32_t i = 0; i < size; ++i) {
            if (_currentUtfOffset != 0 || i > 0) {
                _currentXor *= _increment;
            }
            buffer[i] ^= _currentXor;
            ++_currentUtfOffset;
        }
    }

    uint8_t CUtfReader::PeekUInt8(IStream *stream, uint64_t streamOffset, uint64_t utfOffset) {
        auto value = CBinaryReader::PeekUInt8(stream, streamOffset + utfOffset);
        if (!IsEncrypted()) {
            return value;
        }
        if (utfOffset < _currentUtfOffset) {
            _currentUtfOffset = 0;
        }
        if (_currentUtfOffset == 0) {
            _currentXor = _seed;
        }
        for (auto j = _currentUtfOffset; j < utfOffset; ++j) {
            if (j > 0) {
                _currentXor *= _increment;
            }
            ++_currentUtfOffset;
        }
        if (_currentUtfOffset != 0) {
            _currentXor *= _increment;
        }
        value ^= _currentXor;
        ++_currentUtfOffset;
        return value;
    }

    int8_t CUtfReader::PeekInt8(IStream *stream, uint64_t streamOffset, uint64_t utfOffset) {
        const auto u = PeekUInt8(stream, streamOffset, utfOffset);
        return *(int8_t *)&u;
    }

    float CUtfReader::PeekSingle(IStream *stream, uint64_t streamOffset, uint64_t utfOffset) {
        const auto i = PeekInt32(stream, streamOffset, utfOffset);
        return *(float *)&i;
    }

    double CUtfReader::PeekDouble(IStream *stream, uint64_t streamOffset, uint64_t utfOffset) {
        const auto i = PeekInt64(stream, streamOffset, utfOffset);
        return *(double *)&i;
    }

    template<class T>
    static void reverse(T *array, size_t count) {
        for (auto i = 0; i < count / 2; ++i) {
            const auto t = array[i];
            array[i] = array[count - 1 - i];
            array[count - 1 - i] = t;
        }
    }

#define PEEK_FUNC(bit, u, U) \
    u##int##bit##_t CUtfReader::Peek##U##Int##bit(IStream *stream, uint64_t streamOffset, uint64_t utfOffset) { \
        uint8_t temp[((bit) / 8)]; \
        PeekBytes(stream, temp, streamOffset, ((bit) / 8), utfOffset); \
        if (CBitConverter::IsLittleEndian()) { \
            reverse(temp, ((bit) / 8)); \
        } \
        return CBitConverter::To##U##Int##bit(temp); \
    }

    PEEK_FUNC(16, ,)

    PEEK_FUNC(16, u, U)

    PEEK_FUNC(32, ,)

    PEEK_FUNC(32, u, U)

    PEEK_FUNC(64, ,)

    PEEK_FUNC(64, u, U)

CGSS_NS_END
