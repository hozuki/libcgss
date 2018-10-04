#pragma once

#include "../cgss_env.h"
#include "../takamori/streams/IStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CUtfReader final {

    __root_class(CUtfReader);

    public:

        CUtfReader();

        CUtfReader(uint8_t seed, uint8_t increment);

        bool_t IsEncrypted() const;

        void PeekBytes(IStream *stream, uint8_t *buffer, uint64_t streamOffset, uint32_t size, uint64_t utfOffset);

        void PeekBytes(IStream *stream, uint8_t *buffer, uint64_t bufferOffset, uint64_t streamOffset, uint32_t size, uint64_t utfOffset);

        uint8_t PeekUInt8(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        int8_t PeekInt8(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        uint16_t PeekUInt16(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        int16_t PeekInt16(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        uint32_t PeekUInt32(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        int32_t PeekInt32(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        uint64_t PeekUInt64(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        int64_t PeekInt64(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        float PeekSingle(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

        double PeekDouble(IStream *stream, uint64_t streamOffset, uint64_t utfOffset);

    private:

        const bool_t _encrypted;
        const uint8_t _seed;
        const uint8_t _increment;

        uint8_t _currentXor;
        uint64_t _currentUtfOffset;
        uint8_t _currentStringXor;
        uint64_t _currentUtfStringOffset;

    };

CGSS_NS_END
