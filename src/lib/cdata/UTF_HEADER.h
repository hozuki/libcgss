#pragma once

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

typedef struct _UTF_HEADER {

    uint32_t tableSize;
    uint16_t unk1;
    uint32_t perRowDataOffset;
    uint32_t stringTableOffset;
    uint32_t extraDataOffset;
    uint32_t tableNameOffset;
    uint16_t fieldCount;
    uint16_t rowSize;
    uint32_t rowCount;

} UTF_HEADER;

#pragma pack(pop)
