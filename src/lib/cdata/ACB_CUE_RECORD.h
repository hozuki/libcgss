#pragma once

#include "../cgss_env.h"

#define ACB_CUE_RECORD_NAME_MAX_LEN (256)

struct ACB_CUE_RECORD {

    uint32_t cueId;
    uint8_t referenceType;
    uint16_t referenceIndex;

    bool_t isWaveformIdentified;
    uint16_t waveformIndex;
    uint16_t waveformId;
    uint8_t encodeType;
    bool_t isStreaming;

    char cueName[ACB_CUE_RECORD_NAME_MAX_LEN];

};
