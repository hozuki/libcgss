#pragma once

#include "../cgss_env.h"
#include "../cgss_cenum.h"

#pragma pack(push)
#pragma pack(1)

typedef struct _HCA_CIPHER_CONFIG {

    union {
        struct {
            uint32_t key2;
            uint32_t key1;
        } keyParts;
        uint64_t key;
    };

    CGSS_HCA_CIPHER_TYPE cipherType;

    uint16_t keyModifier;

} HCA_CIPHER_CONFIG;

#pragma pack(pop)
