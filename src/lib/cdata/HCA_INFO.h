#pragma once

#include "../cgss_env.h"
#include "../cgss_cenum.h"

typedef struct _HCA_INFO {
    /**
     * Major part of version number.
     */
    uint16_t versionMajor;
    /**
     * Minor part of version number.
     */
    uint16_t versionMinor;
    /**
     * Channel count.
     */
    uint32_t channelCount;
    /**
     * Sampling rate, in hertz.
     */
    uint32_t samplingRate;
    /**
     * Total number of data blocks.
     */
    uint32_t blockCount;
    /**
     * Size of each data block.
     */
    uint16_t blockSize;
    /**
     * ATH type.
     * Possible values (observed) are 0 and 1.
     */
    uint16_t athType;
    /**
     * Whether internal loop flag is on.
     */
    bool_t loopExists;
    /**
     * The block index at the start of looping segment.
     */
    uint32_t loopStart;
    /**
     * The block index at the end of looping segment.
     */
    uint32_t loopEnd;
    /**
     * Cipher type.
     */
    CGSS_HCA_CIPHER_TYPE cipherType;
    /**
     * Relative volume in RVA (relative volume adjustment) table. It is the multiply factor for floating point wave data.
     */
    float rvaVolume;
    /**
     * Length of comment.
     */
    uint8_t commentLength;
    /**
     * Comment data.
     */
    char comment[0x100];
    uint16_t fmtR01, fmtR02;
    uint16_t compR01, compR02, compR03, compR04, compR05, compR06, compR07, compR08;
    uint32_t compR09;
    uint16_t vbrR01, vbrR02;
    uint16_t loopR01, loopR02;
    /**
     * Offset of audio data. It usually equals to the size of HCA header.
     */
    uint32_t dataOffset;
} HCA_INFO;
