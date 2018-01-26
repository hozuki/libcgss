#pragma once

#include "../cgss_env.h"
#include "../cgss_cenum.h"

#define UTF_FIELD_MAX_NAME_LEN (1024)

typedef struct _UTF_FIELD {

    CGSS_UTF_COLUMN_TYPE type;
    CGSS_UTF_COLUMN_STORAGE storage;

    uint32_t offset;
    uint32_t offsetInRow;

    char name[UTF_FIELD_MAX_NAME_LEN];

    union {
        uint8_t u8;
        int8_t s8;
        uint16_t u16;
        int16_t s16;
        uint32_t u32;
        int32_t s32;
        uint64_t u64;
        int64_t s64;
        float r32;
        double r64;
        struct {
            void *ptr;
            uint32_t size;
        } data;
        char *str;
    } value;

} UTF_FIELD;
