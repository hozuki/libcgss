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

    inline int16_t bswap(int16_t v) {
        int16_t r = (int16_t)(v & 0xFF);
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        return r;
    }

    inline uint16_t bswap(uint16_t v) {
        uint16_t r = (uint16_t)(v & 0xFF);
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        return r;
    }

    inline int32_t bswap(int32_t v) {
        int32_t r = v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        return r;
    }

    inline uint32_t bswap(uint32_t v) {
        uint32_t r = v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        return r;
    }

    inline int64_t bswap(int64_t v) {
        int64_t r = v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        return r;
    }

    inline uint64_t bswap(uint64_t v) {
        uint64_t r = v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        r <<= 8;
        v >>= 8;
        r |= v & 0xFF;
        return r;
    }

    inline float bswap(float v) {
        uint32_t i = bswap(*(uint32_t *)&v);
        return *(float *)&i;
    }

    inline uint32_t ceil2(uint32_t a, uint32_t b) {
        return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0;
    }



CGSS_NS_END
