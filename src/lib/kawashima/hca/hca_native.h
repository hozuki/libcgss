#pragma once

#include "../../cgss_env.h"

#pragma pack(push)
#pragma pack(1)

CGSS_NS_BEGIN

    /**
     * File information (required)
     */
    struct HCA_FILE_HEADER {
        /**
         * Magic: 'hca' XORed with 0x80
         */
        uint32_t hca;
        /**
         * Version. This field is used for determining whether the file is HCA v1.3 or HCA v2.0.
         */
        uint16_t version;
        /**
         * Audio data offset.
         */
        uint16_t dataOffset;
    };

    /**
     * Format information (required)
     */
    struct HCA_FORMAT_HEADER {
        /**
         * Magic: 'fmt' XORed with 0x80
         */
        uint32_t fmt;
        /**
         * Channel count (1-16)
         */
        uint32_t channelCount:8;
        /**
         * Sampling rate (1-0x7fffff)
         */
        uint32_t samplingRate:24;
        /**
         * Block count (>0)
         */
        uint32_t blockCount;
        /**
         * Silenced blocks in the front of this file. It is equals to $block_count * 0x400 + 0x80.
         */
        uint16_t r01;
        /**
         * Silenced blocks in the end of this file. (unverified)
         * Calculation method is unknown. (0x226)
         */
        uint16_t r02;
    };

    /**
     * Compress information. (At least one between compress information and decode information is required.)
     */
    struct HCA_COMPRESS_HEADER {
        /**
         * Magic: 'comp' XORed with 0x80
         */
        uint32_t comp;
        /**
         * Block size. (Is it only enabled when encoded in CBR?)
         * Valid values are from 8 to 0xffff. When it is set to 0, it means the file is encoded in VBR.
         */
        uint16_t blockSize;
        /**
         * Unknown (0-r02)
         * The value is 1 in v2.0.
         */
        uint8_t r01;
        /**
         * Unknown (r01-0x1f)
         * The value is 15 in v2.0.
         */
        uint8_t r02;
        /**
         * Unknown (1)(1)
         */
        uint8_t r03;
        /**
         * Unknown (1)(0)
         */
        uint8_t r04;
        /**
         * Unknown (0x80)(0x80)
         */
        uint8_t r05;
        /**
         * Unknown (0x80)(0x20)
         */
        uint8_t r06;
        /**
         * Unknown (0)(0x20)
         */
        uint8_t r07;
        /**
         * Unknown (0)(0x20)
         */
        uint8_t r08;
        /**
         * Reserved.
         */
        uint8_t reserved1;
        /**
         * Reserved.
         */
        uint8_t reserved2;
    };

    /**
     * Decode information. (At least one between compress information and decode information is required.)
     */
    struct HCA_DECODE_HEADER {
        /**
         * Magic: 'dec' XORed with 0x80
         */
        uint32_t dec;
        /**
         * Block size. (Is it only enabled when encoded in CBR?)
         * Valid values are from 8 to 0xffff. When it is set to 0, it means the file is encoded in VBR.
         */
        uint16_t blockSize;
        /**
         * Unknown (0-r02)
         * The value is 1 in v2.0.
         */
        uint8_t r01;
        /**
         * Unknown (r01-0x1f)
         * The value is 15 in v2.0.
         */
        uint8_t r02;
        /**
         * (Number of type0) + (Number of type1) - 1
         */
        uint8_t count1;
        /**
         * (Number of type2) - 1
         */
        uint8_t count2;
        /**
         * Unknown (0)
         */
        uint8_t r03:4;
        /**
         * Unknown
         * If the value is 0, it is corrected from 1.
         */
        uint8_t r04:4;
        /**
         * The flag showing whether count2 is enabled.
         */
        uint8_t enableCount2;
    };

    /**
     * VBR information. (Obsoleted?)
     */
    struct HCA_VBR_HEADER {
        /**
         * Magic: 'vbr' XORed with 0x80
         */
        uint32_t vbr;
        /**
         * Unknown (0-0x1ff)
         */
        uint16_t r01;
        /**
         * Unknown
         */
        uint16_t r02;
    };

    /**
     * ATH table information. (Obsoleted from v2.0?)
     */
    struct HCA_ATH_HEADER {
        /**
         * Magic: 'ath' XORed with 0x80
         */
        uint32_t ath;
        /**
         * Table type.
         * 0: All elements are 0.
         * 1: Table #1.
         */
        uint16_t type;
    };

    /**
     * Loop information.
     */
    struct HCA_LOOP_HEADER {
        /**
         * Magic: 'loop' XORed with 0x80
         */
        uint32_t loop;
        /**
         * Block index at loop start. (0 to loopEnd)
         */
        uint32_t loopStart;
        /**
         * Block index at loop end. (loopStart to HCA_FORMAT::blockCount - 1)
         */
        uint32_t loopEnd;
        /**
         * Unknown (0x80)
         * Loop flag? Loop times?
         */
        uint16_t r01;
        /**
         * Unknown (0x226)
         */
        uint16_t r02;
    };

    /**
     * Cipher table information.
     */
    struct HCA_CIPHER_HEADER {
        /**
         * Magic: 'loop' XORed with 0x80
         */
        uint32_t ciph;
        /**
         * Cipher type.
         * 0: No cipher.
         * 1: Cipher without a key (by static table).
         * 0x38: Cipher with a key.
         */
        uint16_t type;
    };

    /**
     * Relative volume adjustment information.
     */
    struct HCA_RVA_HEADER {
        /**
         * Magic: 'rva' XORed with 0x80
         */
        uint32_t rva;
        /**
         * Volume.
         */
        float volume;
    };

    /**
     * Comment information.
     */
    struct HCA_COMMENT_HEADER {
        /**
         * Magic: 'comm' XORed with 0x80
         */
        uint32_t comm;
        /**
         * Comment length?
         */
        uint8_t length;
        char comment[0];
    };

    /**
     * Padding.
     */
    struct HCA_PADDING_HEADER {
        /**
         * Magic: 'pad' XORed with 0x80
         */
        uint32_t pad;
        char padding[0];
    };

CGSS_NS_END

#pragma pack(pop)
