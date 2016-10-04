#ifndef KAWASHIMA_HCA_INFO_H
#define KAWASHIMA_HCA_INFO_H

#include "cgss_typedef.h"
#include <string.h>

typedef enum _HCA_CIPHER_TYPE {
    HCA_CIPHER_TYPE_NO_CIPHER = 0,
    HCA_CIPHER_TYPE_STATIC = 1,
    HCA_CIPHER_TYPE_WITH_KEY = 0x38,
    HCA_CIPHER_TYPE_FORCE_DWORD = 0xffffffff
} HCA_CIPHER_TYPE;

typedef enum _HCA_DECODE_FEATURE {
    HCA_DECODE_FEATURE_NONE = 0,
    HCA_DECODE_FEATURE_STREAMING = 1,
    HCA_DECODE_FEATURE_FORCE_DWORD = 0xffffffff
} HCA_DECODE_FEATURE;

typedef struct _HCA_INFO {
    /**
     * Major part of version number.
     */
    uint16 versionMajor;
    /**
     * Minor part of version number.
     */
    uint16 versionMinor;
    /**
     * Channel count.
     */
    uint32 channelCount;
    /**
     * Sampling rate, in hertz.
     */
    uint32 samplingRate;
    /**
     * Total number of data blocks.
     */
    uint32 blockCount;
    /**
     * Size of each data block.
     */
    uint16 blockSize;
    /**
     * ATH type.
     * Possible values (observed) are 0 and 1.
     */
    uint16 athType;
    /**
     * Whether internal loop flag is on.
     */
    ubool loopExists;
    /**
     * The block index at the start of looping segment.
     */
    uint32 loopStart;
    /**
     * The block index at the end of looping segment.
     */
    uint32 loopEnd;
    /**
     * Cipher type.
     */
    HCA_CIPHER_TYPE cipherType;
    /**
     * Relative volume in RVA (relative volume adjustment) table. It is the multiply factor for floating point wave data.
     */
    float rvaVolume;
    /**
     * Length of comment.
     */
    uint8 commentLength;
    /**
     * Comment data.
     */
    char comment[0x100];
    uint16 fmtR01, fmtR02;
    uint16 compR01, compR02, compR03, compR04, compR05, compR06, compR07, compR08;
    uint32 compR09;
    uint16 vbrR01, vbrR02;
    uint16 loopR01, loopR02;
} HCA_INFO;

typedef struct _HCA_CIPHER_CONFIG {

    _HCA_CIPHER_CONFIG() {
        memset(this, 0, sizeof(_HCA_CIPHER_CONFIG));
    }

    _HCA_CIPHER_CONFIG(uint32 key1, uint32 key2) {
        if (key1 == 0 && key2 == 0) {
            cipherType = HCA_CIPHER_TYPE_NO_CIPHER;
        } else {
            cipherType = HCA_CIPHER_TYPE_WITH_KEY;
        }
        keyParts.key1 = key1;
        keyParts.key2 = key2;
    }

    _HCA_CIPHER_CONFIG(uint64 key)
            : _HCA_CIPHER_CONFIG((uint32)(key >> 32), (uint32)(key & 0xffffffff)) {
    };

    union {
        struct {
            uint32 key2;
            uint32 key1;
        } keyParts;
        uint64 key;
    };

    HCA_CIPHER_TYPE cipherType;

} HCA_CIPHER_CONFIG;

#endif //KAWASHIMA_HCA_INFO_H
