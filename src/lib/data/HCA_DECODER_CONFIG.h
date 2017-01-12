#pragma once

#include "../cgss_env.h"
#include "HCA_CIPHER_CONFIG.h"

typedef void (*HcaDecodeFunc)(float data, uint8_t *buffer, uint32_t *cursor);

#pragma pack(push)
#pragma pack(1)

typedef struct _HCA_DECODER_CONFIG {

    HCA_CIPHER_CONFIG cipherConfig;
    bool_t waveHeaderEnabled;
    bool_t loopEnabled;
    uint32_t loopCount;
    HcaDecodeFunc decodeFunc;

} HCA_DECODER_CONFIG;

#pragma pack(pop)
