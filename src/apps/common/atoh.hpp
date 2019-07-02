#pragma once

#ifdef __cplusplus

#if defined(_WIN32) || defined(WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#endif

#include <algorithm>

namespace cgss {

    template<typename T>
    T atoh(const char *str, size_t max_length) {
        max_length = std::min(static_cast<size_t>(max_length), sizeof(T) * 2);

        int i = 0;
        T ret = 0;

        if (str) {
            while (i < max_length && *str) {
                if ('0' <= (*str) && (*str) <= '9') {
                    ret = (ret << 4u) | (uint32_t)(*str - '0');
                } else if ('A' <= (*str) && (*str) <= 'F') {
                    ret = (ret << 4u) | (uint32_t)(*str - 'A' + 10);
                } else if ('a' <= (*str) && (*str) <= 'f') {
                    ret = (ret << 4u) | (uint32_t)(*str - 'a' + 10);
                } else {
                    break;
                }

                ++str;
            }
        }

        return ret;
    }

    template<typename T>
    T atoh(const char *str) {
        return atoh<T>(str, 8);
    }

}

#endif
