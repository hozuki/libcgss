#pragma once

#include <stdint.h>

#define AFS2_FILE_NAME_MAX_LEN (256)

#pragma pack(push)
#pragma pack(1)

typedef struct _AFS2_FILE_RECORD {

    uint16_t cueId;
    uint64_t fileOffsetRaw;
    uint64_t fileOffsetAligned;
    uint64_t fileSize;
    char fileName[AFS2_FILE_NAME_MAX_LEN];

} AFS2_FILE_RECORD;

#pragma pack(pop)
