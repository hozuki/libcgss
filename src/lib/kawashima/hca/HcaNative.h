#ifndef KAWASHIMA_HCANATIVE_H
#define KAWASHIMA_HCANATIVE_H

#include "../kstypedef.h"

/**
 * File information (required)
 */
struct HCA_HEADER {
    /**
     * Magic: 'hca' XORed with 0x80
     */
    uint32 hca;
    /**
     * Version. This field is used for determining whether the file is HCA v1.3 or HCA v2.0.
     */
    uint16 version;
    /**
     * Audio data offset.
     */
    uint16 dataOffset;
};

/**
 * Format information (required)
 */
struct HCA_FORMAT {
    /**
     * Magic: 'fmt' XORed with 0x80
     */
    uint32 fmt;
    /**
     * Channel count (1-16)
     */
    uint32 channelCount:8;
    /**
     * Sampling rate (1-0x7fffff)
     */
    uint32 samplingRate:24;
    /**
     * Block count (>0)
     */
    uint32 blockCount;
    /**
     * Silenced blocks in the front of this file. It is equals to $block_count * 0x400 + 0x80.
     */
    uint16 r01;
    /**
     * Silenced blocks in the end of this file. (unverified)
     * Calculation method is unknown. (0x226)
     */
    uint16 r02;
};

/**
 * Compress information. (At least one between compress information and decode information is required.)
 */
struct HCA_COMPRESS {
    /**
     * Magic: 'comp' XORed with 0x80
     */
    uint32 comp;
    /**
     * Block size. (Is it only enabled when encoded in CBR?)
     * Valid values are from 8 to 0xffff. When it is set to 0, it means the file is encoded in VBR.
     */
    uint16 blockSize;
    /**
     * Unknown (0-r02)
     * The value is 1 in v2.0.
     */
    uint8 r01;
    /**
     * Unknown (r01-0x1f)
     * The value is 15 in v2.0.
     */
    uint8 r02;
    /**
     * Unknown (1)(1)
     */
    uint8 r03;
    /**
     * Unknown (1)(0)
     */
    uint8 r04;
    /**
     * Unknown (0x80)(0x80)
     */
    uint8 r05;
    /**
     * Unknown (0x80)(0x20)
     */
    uint8 r06;
    /**
     * Unknown (0)(0x20)
     */
    uint8 r07;
    /**
     * Unknown (0)(0x20)
     */
    uint8 r08;
    /**
     * Reserved.
     */
    uint8 reserved1;
    /**
     * Reserved.
     */
    uint8 reserved2;
};

/**
 * Decode information. (At least one between compress information and decode information is required.)
 */
struct HCA_DECODE {
    /**
     * Magic: 'dec' XORed with 0x80
     */
    uint32 dec;
    /**
     * Block size. (Is it only enabled when encoded in CBR?)
     * Valid values are from 8 to 0xffff. When it is set to 0, it means the file is encoded in VBR.
     */
    uint16 blockSize;
    /**
     * Unknown (0-r02)
     * The value is 1 in v2.0.
     */
    uint8 r01;
    /**
     * Unknown (r01-0x1f)
     * The value is 15 in v2.0.
     */
    uint8 r02;
    /**
     * (Number of type0) + (Number of type1) - 1
     */
    uint8 count1;
    /**
     * (Number of type2) - 1
     */
    uint8 count2;
    /**
     * Unknown (0)
     */
    uint8 r03:4;
    /**
     * Unknown
     * If the value is 0, it is corrected from 1.
     */
    uint8 r04:4;
    /**
     * The flag showing whether count2 is enabled.
     */
    uint8 enableCount2;
};

/**
 * VBR information. (Obsoleted?)
 */
struct HCA_VBR {
    /**
     * Magic: 'vbr' XORed with 0x80
     */
    uint32 vbr;
    /**
     * Unknown (0-0x1ff)
     */
    uint16 r01;
    /**
     * Unknown
     */
    uint16 r02;
};

/**
 * ATH table information. (Obsoleted from v2.0?)
 */
struct HCA_ATH {
    /**
     * Magic: 'ath' XORed with 0x80
     */
    uint32 ath;
    /**
     * Table type.
     * 0: All elements are 0.
     * 1: Table #1.
     */
    uint16 type;
};

/**
 * Loop information.
 */
struct HCA_LOOP {
    /**
     * Magic: 'loop' XORed with 0x80
     */
    uint32 loop;
    /**
     * Block index at loop start. (0 to loopEnd)
     */
    uint32 loopStart;
    /**
     * Block index at loop end. (loopStart to HCA_FORMAT::blockCount - 1)
     */
    uint32 loopEnd;
    /**
     * Unknown (0x80)
     * Loop flag? Loop times?
     */
    uint16 r01;
    /**
     * Unknown (0x226)
     */
    uint16 r02;
};

/**
 * Cipher table information.
 */
struct HCA_CIPHER {
    /**
     * Magic: 'loop' XORed with 0x80
     */
    uint32 ciph;
    /**
     * Cipher type.
     * 0: No cipher.
     * 1: Cipher without a key (by static table).
     * 0x38: Cipher with a key.
     */
    uint16 type;
};

/**
 * Relative volume adjustment information.
 */
struct HCA_RVA {
    /**
     * Magic: 'rva' XORed with 0x80
     */
    uint32 rva;
    /**
     * Volume.
     */
    float volume;
};

/**
 * Comment information.
 */
struct HCA_COMMENT {
    /**
     * Magic: 'comm' XORed with 0x80
     */
    uint32 comm;
    /**
     * Comment length?
     */
    uint8 len;
    char comment[0];
};

/**
 * Padding.
 */
struct HCA_PADDING {
    /**
     * Magic: 'pad' XORed with 0x80
     */
    uint32 pad;
};

#endif //KAWASHIMA_HCANATIVE_H
