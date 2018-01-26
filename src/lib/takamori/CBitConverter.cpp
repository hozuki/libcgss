#include "CBitConverter.h"

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

    bool_t CBitConverter::IsLittleEndian() {
        return static_cast<bool_t>(O32_HOST_ORDER == O32_LITTLE_ENDIAN);
    }

    float CBitConverter::ToSingle(const void *p) {
        const auto v = ToInt32(p);
        return *(float *)&v;
    }

    double CBitConverter::ToDouble(const void *p) {
        const auto v = ToInt64(p);
        return *(double *)&v;
    }

#define TO_INT(bit, u, U) \
    u##int##bit##_t CBitConverter::To##U##Int##bit(const void *p) { \
        const auto *pb = static_cast<const uint8_t *>(p); \
        u##int##bit##_t v = 0; \
        if (IsLittleEndian()) { \
            for (auto i = 0; i < ((bit) / 8); ++i) { \
                v = v | (pb[i] << (i * 8)); \
            } \
        } else { \
            for (auto i = 0; i < ((bit) / 8); ++i) { \
                v = (v << 8) | pb[i]; \
            } \
        }; \
        return v; \
    }

    TO_INT(16, ,)

    TO_INT(16, u, U)

    TO_INT(32, ,)

    TO_INT(32, u, U)

    TO_INT(64, ,)

    TO_INT(64, u, U)

CGSS_NS_END
