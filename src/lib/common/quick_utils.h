#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    template<typename T>
    void clone(const T &src, T &dst) {
        memcpy(&dst, &src, sizeof(T));
    }

    template<typename T>
    T clamp(T value, T min, T max) {
        return value < min ? min : (value > max ? max : value);
    }

    inline uint16_t bswap(uint16_t v) {
#if defined(__GNUC__)
        return __builtin_bswap16(v);
#elif defined(_MSC_VER)
        return _byteswap_ushort(v);
#else
        uint16_t r = (uint16_t)(v & 0xffu);
        r <<= 8;
        v >>= 8;
        r |= v & 0xffu;
        return r;
#endif
    }

    inline int16_t bswap(int16_t v) {
        auto v2 = *(uint16_t *)&v;
        v2 = bswap(v2);
        return *(int16_t *)&v2;
    }

    inline uint32_t bswap(uint32_t v) {
#if defined(__GNUC__)
        return __builtin_bswap32(v);
#elif defined(_MSC_VER)
        return _byteswap_ulong(v);
#else
        uint32_t r = v & 0xffu;

        for (auto i = 0; i < 3; i += 1) {
            r <<= 8;
            v >>= 8;
            r |= v & 0xffu;
        }

        return r;
#endif
    }

    inline int32_t bswap(int32_t v) {
        auto v2 = *(uint32_t *)&v;
        v2 = bswap(v2);
        return *(int32_t *)&v2;
    }

    inline uint64_t bswap(uint64_t v) {
#if defined(__GNUC__)
        return __builtin_bswap64(v);
#elif defined(_MSC_VER)
        return _byteswap_uint64(v);
#else
        uint64_t r = v & 0xffu;

        for (auto i = 0; i < 7; i += 1) {
            r <<= 8;
            v >>= 8;
            r |= v & 0xffu;
        }

        return r;
#endif
    }

    inline int64_t bswap(int64_t v) {
        auto v2 = *(uint64_t *)&v;
        v2 = bswap(v2);
        return *(int64_t *)&v2;
    }

    inline float bswap(float v) {
        uint32_t i = bswap(*(uint32_t *)&v);
        return *(float *)&i;
    }

    inline double bswap(double v) {
        uint64_t i = bswap(*(uint64_t *)&v);
        return *(double *)&i;
    }

    inline uint32_t ceil2(uint32_t a, uint32_t b) {
        return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0;
    }


CGSS_NS_END
