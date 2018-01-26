#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CBitConverter final {

    PURE_STATIC(CBitConverter);

    public:

        static bool_t IsLittleEndian();

        static int16_t ToInt16(const void *p);

        static uint16_t ToUInt16(const void *p);

        static int32_t ToInt32(const void *p);

        static uint32_t ToUInt32(const void *p);

        static int64_t ToInt64(const void *p);

        static uint64_t ToUInt64(const void *p);

        static float ToSingle(const void *p);

        static double ToDouble(const void *p);

    };

CGSS_NS_END
