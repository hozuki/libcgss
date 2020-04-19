#pragma once

#include <stdint.h>

#include "bool_t.h"
#include "HCA_CIPHER_CONFIG.h"

#ifdef __cplusplus
typedef uint32_t (*HcaDecodeFunc)(float data, uint8_t *buffer, const uint32_t cursor);
#else
typedef uint32_t (*HcaDecodeFunc)(float data, uint8_t *buffer, uint32_t cursor);
#endif

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
