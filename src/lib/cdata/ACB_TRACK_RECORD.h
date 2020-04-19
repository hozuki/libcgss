#pragma once

#include <stdint.h>

#include "bool_t.h"

#pragma pack(push)
#pragma pack(1)

typedef struct _ACB_TRACK_RECORD {

    uint32_t trackIndex;
    uint16_t synthIndex;

    bool_t isWaveformIdentified;
    uint16_t waveformIndex;
    uint16_t waveformId;
    uint8_t encodeType;
    bool_t isStreaming;

} ACB_TRACK_RECORD;

#pragma pack(pop)
