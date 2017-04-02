#pragma once

#include "../../cgss_env.h"
#include "../../takamori/exceptions/CFormatException.h"

CGSS_NS_BEGIN

    struct WaveSettings final {
        /**
         * Bit per channel. Future acceptable values will be 8, 16, 24, 32 and 0 (floating point wave data).
         */
        static const uint32_t BitPerChannel;
        static const bool_t SoftLoop;

    PURE_STATIC(WaveSettings);

    };

    enum class Magic : uint32_t {

        HCA = 0x00414348,
        FORMAT = 0x00746D66,
        COMPRESS = 0x706D6F63,
        DECODE = 0x00636564,
        VBR = 0x00726276,
        ATH = 0x00687461,
        LOOP = 0x706F6F6C,
        CIPHER = 0x68706963,
        RVA = 0x00617672,
        COMMENT = 0x6D6D6F63

    };

    inline bool_t areMagicMatch(uint32_t toCheck, Magic standard) {
        return static_cast<bool_t>((toCheck & 0x7f7f7f7f) == static_cast<std::underlying_type_t<Magic>>(standard));
    }

    inline void ensureMagicMatch(uint32_t toCheck, Magic standard) {
        if (!areMagicMatch(toCheck, standard)) {
            throw CFormatException();
        }
    }

CGSS_NS_END
