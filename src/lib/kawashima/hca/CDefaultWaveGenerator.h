#pragma once

#include "../../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CDefaultWaveGenerator final {

    public:

        static uint32_t Decode8BitU(float data, uint8_t *buffer, uint32_t cursor);

        static uint32_t Decode16BitS(float data, uint8_t *buffer, uint32_t cursor);

        static uint32_t Decode24BitS(float data, uint8_t *buffer, uint32_t cursor);

        static uint32_t Decode32BitS(float data, uint8_t *buffer, uint32_t cursor);

        static uint32_t DecodeFloat(float data, uint8_t *buffer, uint32_t cursor);

    PURE_STATIC(CDefaultWaveGenerator);

    };

CGSS_NS_END
