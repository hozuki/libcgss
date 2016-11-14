#define _CRT_SECURE_NO_WARNINGS

#include <memory.h>
#include <string.h>
#include <cstdio>
#include "CHcaDecoder.h"
#include "HcaNative.h"
#include "Magic.h"
#include "WaveGen.h"
#include "../wave/WaveNative.h"
#include "../../hca_info.hpp"

typedef struct _WAVE_SETTINGS {
    /**
     * Bit per channel. Future acceptable values will be 8, 16, 24, 32 and 0 (floating point wave data).
     */
    uint32 bitPerChannel;
    ubool useLoop;
} WAVE_SETTINGS;

static WAVE_SETTINGS waveSettings = {16, FALSE};

template<typename T>
void clone(const T &src, T &dst) {
    memcpy(&dst, &src, sizeof(T));
}

template<typename T>
T clamp(T value, T min, T max) {
    return value < min ? min : (value > max ? max : value);
}

inline int16 bswap(int16 v) {
    int16 r = (int16)(v & 0xFF);
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    return r;
}

inline uint16 bswap(uint16 v) {
    uint16 r = (uint16)(v & 0xFF);
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    return r;
}

inline int32 bswap(int32 v) {
    int32 r = v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    return r;
}

inline uint32 bswap(uint32 v) {
    uint32 r = v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    return r;
}

inline int64 bswap(int64 v) {
    int64 r = v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    return r;
}

inline uint64 bswap(uint64 v) {
    uint64 r = v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    r <<= 8;
    v >>= 8;
    r |= v & 0xFF;
    return r;
}

inline float bswap(float v) {
    uint32 i = bswap(*(uint32 *)&v);
    return *(float *)&i;
}

inline uint32 ceil2(uint32 a, uint32 b) {
    return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0;
}

const uint16 *CHcaDecoder::ChecksumTable = new uint16[256]{
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202,
};

CHcaDecoder::CHcaDecoder()
        : CHcaDecoder(HCA_CIPHER_CONFIG()) {
}

CHcaDecoder::CHcaDecoder(uint32 key1, uint32 key2) {
    _ccFrom.cipherType = !(key1 | key2) ? HCA_CIPHER_TYPE_NO_CIPHER : HCA_CIPHER_TYPE_WITH_KEY;
    _ccFrom.keyParts.key1 = key1;
    _ccFrom.keyParts.key2 = key2;
    memset(&_ccTo, 0, sizeof(HCA_CIPHER_CONFIG));
}

CHcaDecoder::CHcaDecoder(const HCA_CIPHER_CONFIG &crypt)
        : CHcaDecoder(crypt, HCA_CIPHER_CONFIG()) {
}

CHcaDecoder::CHcaDecoder(const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo)
        : _cipherFrom(), _cipherTo() {
    clone(cryptFrom, _ccFrom);
    clone(cryptTo, _ccTo);
    memset(&_hcaInfo, 0, sizeof(HCA_INFO));
}

uint16 CHcaDecoder::Checksum(void *pData, uint32 dwDataSize, uint16 wInitSum) {
    uint8 *p = (uint8 *)pData;
    for (uint32 i = 0; i < dwDataSize; ++i, ++p) {
        wInitSum = (wInitSum << 8) ^ ChecksumTable[(wInitSum >> 8) ^ *p];
    }
    return wInitSum;
}

KS_RESULT CHcaDecoder::ReadHeader(uint8 *pData, uint32 dwFileSize, uint32 *pdwDataOffset) {
    KS_DECODE_STATUS decodeStatus;
    memset(&decodeStatus, 0, sizeof(KS_DECODE_STATUS));
    auto result = ReadHeader(pData, dwFileSize, &decodeStatus);
    *pdwDataOffset = KS_CALL_SUCCESSFUL(result) ? decodeStatus.dataCursor : 0;
    return result;
}

KS_RESULT CHcaDecoder::ReadHeader(uint8 *pFileData, uint32 dwDataSize, KS_DECODE_STATUS *pStatus) {
    if (!pFileData || !pStatus) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (dwDataSize < sizeof(HCA_HEADER)) {
        return KS_ERR_INVALID_PARAMETER;
    }
    uint32 cursor = pStatus->dataCursor;
    auto &hcaInfo = _hcaInfo;

    // Check HCA header (of the whole file).
    HCA_HEADER *hca = (HCA_HEADER *)(pFileData + cursor);
    if (!Magic::Match(hca->hca, Magic::HCA)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    // The file must be bigger than a raw HCA header.
    uint32 dataOffset = bswap(hca->dataOffset);
    if (dwDataSize < dataOffset) {
        return KS_ERR_INVALID_PARAMETER;
    }
    uint16 fileVersion = bswap(hca->version);
    hcaInfo.versionMajor = (uint16)(fileVersion >> 8);
    hcaInfo.versionMinor = (uint16)(fileVersion & 0xff);
    if (Checksum(hca, dataOffset, 0) != 0) {
        // File may be broken.
        return KS_ERR_CHECKSUM_NOT_MATCH;
    }
    cursor += sizeof(HCA_HEADER);
    hcaInfo.dataOffset = dataOffset;

    // FMT
    HCA_FORMAT *fmt = (HCA_FORMAT *)(pFileData + cursor);
    if (!Magic::Match(fmt->fmt, Magic::FORMAT)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    hcaInfo.channelCount = fmt->channelCount;
    hcaInfo.samplingRate = bswap((uint32)(fmt->samplingRate << 8));
    hcaInfo.blockCount = bswap(fmt->blockCount);
    hcaInfo.fmtR01 = bswap(fmt->r01);
    hcaInfo.fmtR02 = bswap(fmt->r02);
    if (!(hcaInfo.channelCount >= 1 && hcaInfo.channelCount <= 16)) {
        return KS_ERR_INVALID_INTERNAL_STATE;
    }
    if (!(hcaInfo.samplingRate >= 1 && hcaInfo.samplingRate <= 0x7fffff)) {
        return KS_ERR_INVALID_INTERNAL_STATE;
    }
    cursor += sizeof(HCA_FORMAT);

    // COMP or DEC
    if (Magic::Match(*((uint32 *)(pFileData + cursor)), Magic::COMPRESS)) {
        HCA_COMPRESS *comp = (HCA_COMPRESS *)(pFileData + cursor);
        hcaInfo.blockSize = bswap(comp->blockSize);
        hcaInfo.compR01 = comp->r01;
        hcaInfo.compR02 = comp->r02;
        hcaInfo.compR03 = comp->r03;
        hcaInfo.compR04 = comp->r04;
        hcaInfo.compR05 = comp->r05;
        hcaInfo.compR06 = comp->r06;
        hcaInfo.compR07 = comp->r07;
        hcaInfo.compR08 = comp->r08;
        if (!((hcaInfo.blockSize >= 8 && hcaInfo.blockSize <= 0xFFFF) || (hcaInfo.blockSize == 0))) {
            return KS_ERR_INVALID_INTERNAL_STATE;
        }
        if (!(hcaInfo.compR01 >= 0 && hcaInfo.compR01 <= hcaInfo.compR02 && hcaInfo.compR02 <= 0x1f)) {
            return KS_ERR_INVALID_INTERNAL_STATE;
        }
        cursor += sizeof(HCA_COMPRESS);
    } else if (Magic::Match(*((uint32 *)(pFileData + cursor)), Magic::DECODE)) {
        HCA_DECODE *dec = (HCA_DECODE *)(pFileData + cursor);
        hcaInfo.blockSize = bswap(dec->blockSize);
        hcaInfo.compR01 = dec->r01;
        hcaInfo.compR02 = dec->r02;
        hcaInfo.compR03 = dec->r04;
        hcaInfo.compR04 = dec->r03;
        hcaInfo.compR05 = (uint16)(dec->count1 + 1);
        hcaInfo.compR06 = (uint16)((dec->enableCount2 ? dec->count2 : dec->count1) + 1);
        hcaInfo.compR07 = hcaInfo.compR05 - hcaInfo.compR06;
        hcaInfo.compR08 = 0;
        if (!((hcaInfo.blockSize >= 8 && hcaInfo.blockSize <= 0xFFFF) || (hcaInfo.blockSize == 0))) {
            return KS_ERR_INVALID_INTERNAL_STATE;
        }
        if (!(hcaInfo.compR01 >= 0 && hcaInfo.compR01 <= hcaInfo.compR02 && hcaInfo.compR02 <= 0x1f)) {
            return KS_ERR_INVALID_INTERNAL_STATE;
        }
        if (!hcaInfo.compR03) {
            hcaInfo.compR03 = 1;
        }
        cursor += sizeof(HCA_DECODE);
    } else {
        return KS_ERR_MAGIC_NOT_MATCH;
    }

    // VBR
    HCA_VBR *vbr = (HCA_VBR *)(pFileData + cursor);
    if (Magic::Match(vbr->vbr, Magic::VBR)) {
        hcaInfo.vbrR01 = bswap(vbr->r01);
        hcaInfo.vbrR02 = bswap(vbr->r02);
        cursor += sizeof(HCA_VBR);
    } else {
        hcaInfo.vbrR01 = hcaInfo.vbrR02 = 0;
    }

    // ATH
    HCA_ATH *ath = (HCA_ATH *)(pFileData + cursor);
    if (Magic::Match(ath->ath, Magic::ATH)) {
        hcaInfo.athType = ath->type;
        cursor += sizeof(HCA_ATH);
    } else {
        hcaInfo.athType = (uint16)(hcaInfo.versionMajor < 2 ? 1 : 0);
    }

    // LOOP
    HCA_LOOP *loop = (HCA_LOOP *)(pFileData + cursor);
    if (Magic::Match(loop->loop, Magic::LOOP)) {
        hcaInfo.loopExists = TRUE;
        hcaInfo.loopStart = bswap(loop->loopStart);
        hcaInfo.loopEnd = bswap(loop->loopEnd);
        hcaInfo.loopR01 = bswap(loop->r01);
        hcaInfo.loopR02 = bswap(loop->r02);
        if (!(0 <= hcaInfo.loopStart && hcaInfo.loopStart <= hcaInfo.loopEnd &&
              hcaInfo.loopEnd < hcaInfo.blockCount)) {
            return KS_ERR_INVALID_INTERNAL_STATE;
        }
        cursor += sizeof(HCA_LOOP);
    } else {
        hcaInfo.loopStart = hcaInfo.loopEnd = 0;
        hcaInfo.loopR01 = 0;
        hcaInfo.loopR02 = 0x400;
        hcaInfo.loopExists = FALSE;
    }

    // CIPH
    HCA_CIPHER *ciph = (HCA_CIPHER *)(pFileData + cursor);
    if (Magic::Match(ciph->ciph, Magic::CIPHER)) {
        hcaInfo.cipherType = (HCA_CIPHER_TYPE)bswap(ciph->type);
        if (!(hcaInfo.cipherType == HCA_CIPHER_TYPE_NO_CIPHER || hcaInfo.cipherType == HCA_CIPHER_TYPE_STATIC ||
              hcaInfo.cipherType == HCA_CIPHER_TYPE_WITH_KEY)) {
            return KS_ERR_INVALID_INTERNAL_STATE;
        }
        cursor += sizeof(HCA_CIPHER);
    } else {
        hcaInfo.cipherType = HCA_CIPHER_TYPE_NO_CIPHER;
    }

    // RVA
    HCA_RVA *rva = (HCA_RVA *)(pFileData + cursor);
    if (Magic::Match(rva->rva, Magic::RVA)) {
        hcaInfo.rvaVolume = bswap(rva->volume);
        cursor += sizeof(HCA_RVA);
    } else {
        hcaInfo.rvaVolume = 1.0f;
    }

    // COMM
    HCA_COMMENT *comment = (HCA_COMMENT *)(pFileData + cursor);
    if (Magic::Match(comment->comm, Magic::COMMENT)) {
        hcaInfo.commentLength = comment->len;
        memset(hcaInfo.comment, 0, 0x100);
        strcpy(hcaInfo.comment, comment->comment);

    } else {
        hcaInfo.commentLength = 0;
        memset(hcaInfo.comment, 0, 0x100);
    }

    // Initialize adjustment and cipher tables.
    if (!_ath.Init(hcaInfo.athType, hcaInfo.samplingRate)) {
        return KS_ERR_ATH_INIT_FAILED;
    }
    _ccFrom.cipherType = hcaInfo.cipherType;
    if (!_cipherFrom.Init((HCA_CIPHER_TYPE)hcaInfo.cipherType, _ccFrom.keyParts.key1, _ccFrom.keyParts.key2)) {
        return KS_ERR_CIPH_INIT_FAILED;
    }
    if (!_cipherTo.Init(_ccTo.cipherType, _ccTo.keyParts.key1, _ccTo.keyParts.key2)) {
        return KS_ERR_CIPH_INIT_FAILED;
    }

    if (!hcaInfo.compR03) {
        hcaInfo.compR03 = 1;
    }

    // Prepare the channel decoders.
    memset(_channels, 0, sizeof(_channels));
    if (!(hcaInfo.compR01 == 1 && hcaInfo.compR02 == 0xf)) {
        return KS_ERR_INVALID_INTERNAL_STATE;
    }
    hcaInfo.compR09 = ceil2(hcaInfo.compR05 - (hcaInfo.compR06 + hcaInfo.compR07), hcaInfo.compR08);
    uint8 r[0x10];
    memset(r, 0, 0x10);
    uint32 b = hcaInfo.channelCount / hcaInfo.compR03;
    if (hcaInfo.compR07 && b > 1) {
        uint8 *c = r;
        for (auto i = 0; i < hcaInfo.compR03; ++i, c += b) {
            switch (b) {
                case 2:
                case 3:
                    c[0] = 1;
                    c[1] = 2;
                    break;
                case 4:
                    c[0] = 1;
                    c[1] = 2;
                    if (hcaInfo.compR04 == 0) {
                        c[2] = 1;
                        c[3] = 2;
                    }
                    break;
                case 5:
                    c[0] = 1;
                    c[1] = 2;
                    if (hcaInfo.compR04 <= 2) {
                        c[3] = 1;
                        c[4] = 2;
                    }
                    break;
                case 6:
                case 7:
                    c[0] = 1;
                    c[1] = 2;
                    c[4] = 1;
                    c[5] = 2;
                    // Fall through
                case 8:
                    c[6] = 1;
                    c[7] = 2;
                    break;
                default:
                    return KS_ERR_STATE_OUT_OF_RANGE;
            }
        }
    }
    for (auto i = 0; i < hcaInfo.channelCount; ++i) {
        _channels[i].type = r[i];
        _channels[i].value3 = &_channels[i].value[hcaInfo.compR06 + hcaInfo.compR07];
        _channels[i].count = hcaInfo.compR06 + ((r[i] != 2) ? hcaInfo.compR07 : 0);
    }

    pStatus->dataCursor = dataOffset;
    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::GetInfo(HCA_INFO &info) const {
    return GetInfo(&info);
}

KS_RESULT CHcaDecoder::GetInfo(HCA_INFO *pInfo) const {
    if (pInfo == nullptr) {
        return KS_ERR_INVALID_PARAMETER;
    }
    memcpy(pInfo, &_hcaInfo, sizeof(HCA_INFO));
    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::GetWaveHeader(uint8 *pBuffer, uint32 *pdwWaveHeaderSize) {
    if (!pdwWaveHeaderSize) {
        return KS_ERR_INVALID_PARAMETER;
    }

    int loopCount = 0;
    WaveRiffSection wavRiff = {'R', 'I', 'F', 'F', 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 0x10, 0, 0, 0, 0, 0, 0};
    WaveSampleSection wavSmpl = {'s', 'm', 'p', 'l', 0x3C, 0, 0, 0, 0x3C, 0, 0, 0, 1, 0x18, 0, 0, 0, 0, 0, 0};
    WaveNoteSection wavNote = {'n', 'o', 't', 'e', 0, 0};
    WaveDataSection wavData = {'d', 'a', 't', 'a', 0};
    wavRiff.fmtType = (uint16)((waveSettings.bitPerChannel > 0) ? 1 : 3);
    wavRiff.fmtChannelCount = (uint16)_hcaInfo.channelCount;
    wavRiff.fmtBitCount = (uint16)((waveSettings.bitPerChannel > 0) ? waveSettings.bitPerChannel : 32);
    wavRiff.fmtSamplingRate = _hcaInfo.samplingRate;
    wavRiff.fmtSamplingSize = (uint16)(wavRiff.fmtBitCount / 8 * wavRiff.fmtChannelCount);
    wavRiff.fmtSamplesPerSec = wavRiff.fmtSamplingRate * wavRiff.fmtSamplingSize;
    if (_hcaInfo.loopExists) {
        wavSmpl.samplePeriod = (uint32)(1 / (double)wavRiff.fmtSamplingRate * 1000000000);
        wavSmpl.loopStart = _hcaInfo.loopStart * 0x80 * 8 * wavRiff.fmtSamplingSize;
        wavSmpl.loopEnd = _hcaInfo.loopEnd * 0x80 * 8 * wavRiff.fmtSamplingSize;
        wavSmpl.loopPlayCount = (_hcaInfo.loopR01 == 0x80) ? 0 : _hcaInfo.loopR01;
    } else if (waveSettings.useLoop) {
        wavSmpl.loopStart = 0;
        wavSmpl.loopEnd = _hcaInfo.blockCount * 0x80 * 8 * wavRiff.fmtSamplingSize;
        _hcaInfo.loopStart = 0;
        _hcaInfo.loopEnd = _hcaInfo.blockCount;
    }
    if (_hcaInfo.commentLength > 0) {
        wavNote.noteSize = 4 + _hcaInfo.commentLength + 1;
        if (wavNote.noteSize & 3) {
            wavNote.noteSize += 4 - (wavNote.noteSize & 3);
        }
    }
    wavData.dataSize = _hcaInfo.blockCount * 0x80 * 8 * wavRiff.fmtSamplingSize +
                       (wavSmpl.loopEnd - wavSmpl.loopStart) * loopCount;
    wavRiff.riffSize = (uint32)(0x1C + ((_hcaInfo.loopExists && !waveSettings.useLoop) ? sizeof(wavSmpl) : 0) +
                                (_hcaInfo.commentLength > 0 ? 8 + wavNote.noteSize : 0) + sizeof(wavData) +
                                wavData.dataSize);

    uint32 sizeNeeded = (uint32)sizeof(WaveRiffSection);
    if (_hcaInfo.loopExists && !waveSettings.useLoop) {
        sizeNeeded += sizeof(WaveSampleSection);
    }
    if (_hcaInfo.commentLength > 0) {
        sizeNeeded += 8 + wavNote.noteSize;
    }
    sizeNeeded += sizeof(WaveDataSection);

    // Only computes the needed size of the buffer, and returns.
    if (!pBuffer) {
        *pdwWaveHeaderSize = sizeNeeded;
        return KS_ERR_OK;
    } else {
        ubool bufferLargeEnough = (ubool)(*pdwWaveHeaderSize >= sizeNeeded);
        if (bufferLargeEnough) {
            uint32 cursor = 0;
#define WRITE_STRUCT(src, size) memcpy(pBuffer + cursor, src, size); cursor += size
            WRITE_STRUCT(&wavRiff, sizeof(WaveRiffSection));
            if (_hcaInfo.loopExists && !waveSettings.useLoop) {
                WRITE_STRUCT(&wavSmpl, sizeof(WaveSampleSection));
            }
            if (_hcaInfo.commentLength > 0) {
                uint32 address = cursor;
                WRITE_STRUCT(&wavNote, sizeof(WaveNoteSection));
                strcpy((char *)(pBuffer + cursor), _hcaInfo.comment);
                pBuffer[cursor + _hcaInfo.commentLength] = '\0';
                cursor = address + 8 + wavNote.noteSize;
            }
            WRITE_STRUCT(&wavData, sizeof(WaveDataSection));
#undef WRITE_STRUCT
        }
        *pdwWaveHeaderSize = sizeNeeded;
        return bufferLargeEnough ? KS_ERR_OK : KS_ERR_BUFFER_TOO_SMALL;
    }
}

KS_RESULT CHcaDecoder::DecodeData(uint8 *pData, uint32 dwDataSize, KS_DECODE_STATUS *status, uint8 *pWaveData,
                                  uint32 *pdwWaveDataSize, ubool *pbHasMore) {
    if (!pData || !status || !pdwWaveDataSize || !pbHasMore) {
        return KS_ERR_INVALID_PARAMETER;
    }
    uint32 audioBPC = waveSettings.bitPerChannel != 0 ? waveSettings.bitPerChannel : sizeof(float);
    uint32 waveBlockSize = 0x80 * (audioBPC / sizeof(uint8)) * _hcaInfo.channelCount;

    // The consumer just want to check the minimum acceptable buffer size.
    if (!pWaveData) {
        *pdwWaveDataSize = waveBlockSize;
        *pbHasMore = FALSE;
        return KS_ERR_OK;
    }

    if (*pdwWaveDataSize < waveBlockSize) {
        return KS_ERR_BUFFER_TOO_SMALL;
    }
    if (status->blockIndex >= _hcaInfo.blockCount) {
        return KS_ERR_DECODE_ALREADY_COMPLETED;
    }
    uint32 blocksProcessableThisRound = *pdwWaveDataSize / waveBlockSize;

    if (!waveSettings.useLoop && !_hcaInfo.loopExists) {
        uint32 bufferCursor = 0;
        if (status->blockIndex + blocksProcessableThisRound >= _hcaInfo.blockCount) {
            blocksProcessableThisRound = _hcaInfo.blockCount - status->blockIndex;
            *pbHasMore = FALSE;
        } else {
            *pbHasMore = TRUE;
        }
        KS_RESULT result;
        for (auto i = 0; i < blocksProcessableThisRound; ++i) {
            result = GenerateWaveDataBlock(pData, _hcaInfo.blockSize, &status->dataCursor, pWaveData, &bufferCursor,
                                           WaveGen::Decode16Bit);
            if (!KS_CALL_SUCCESSFUL(result)) {
                return result;
            }
        }
        status->blockIndex += blocksProcessableThisRound;
        *pdwWaveDataSize = bufferCursor;
    } else {
        return KS_ERR_NOT_IMPLEMENTED;
    }

    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::GenerateWaveDataBlock(uint8 *pData, uint32 dwBlockSize, uint32 *pDataCursor, uint8 *pBuffer,
                                             uint32 *pBufferCursor, DecodeFunc pfnDecodeFunc) {
    if (!pData || !pDataCursor || !pBuffer || !pBufferCursor || !pfnDecodeFunc) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_RESULT result = DecodeBlock(pData, dwBlockSize, pDataCursor);
    if (!KS_CALL_SUCCESSFUL(result)) {
        return result;
    }
    float f;
    for (auto i = 0; i < 8; ++i) {
        for (auto j = 0; j < 0x80; ++j) {
            for (auto k = 0; k < _hcaInfo.channelCount; ++k) {
                f = _channels[k].wave[i][j] * _hcaInfo.rvaVolume;
                f = clamp(f, -1.0f, 1.0f);
                pfnDecodeFunc(f, pBuffer, pBufferCursor);
            }
        }
    }
    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::DecodeBlock(uint8 *pData, uint32 dwBlockSize, uint32 *pDataCursor) {
    if (!pData || !pDataCursor) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (dwBlockSize != _hcaInfo.blockSize) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (Checksum(pData + *pDataCursor, dwBlockSize, 0) != 0) {
        return KS_ERR_CHECKSUM_NOT_MATCH;
    }
    _cipherFrom.Decrypt(pData + *pDataCursor, dwBlockSize);
    CHcaData data(pData + *pDataCursor, dwBlockSize);
    int32 magic = data.GetBit(16);
    if (magic == 0xffff) {
        int32 a = (data.GetBit(9) << 8) - data.GetBit(7);
        for (auto i = 0; i < _hcaInfo.channelCount; ++i) {
            _channels[i].Decode1(&data, _hcaInfo.compR09, a, _ath.GetTable());
        }
        for (auto i = 0; i < 8; ++i) {
            for (auto j = 0; j < _hcaInfo.channelCount; ++j) {
                _channels[j].Decode2(&data);
            }
            for (auto j = 0; j < _hcaInfo.channelCount; ++j) {
                _channels[j].Decode3(_hcaInfo.compR09, _hcaInfo.compR08, _hcaInfo.compR07 + _hcaInfo.compR06,
                                     _hcaInfo.compR05);
            }
            for (auto j = 0; j < _hcaInfo.channelCount - 1; ++j) {
                _channels[j].Decode4(i, _hcaInfo.compR05 - _hcaInfo.compR06, _hcaInfo.compR06, _hcaInfo.compR07);
            }
            for (auto j = 0; j < _hcaInfo.channelCount; ++j) {
                _channels[j].Decode5(i);
            }
        }
    } else {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    *pDataCursor += dwBlockSize;
    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::ConvertData(uint8 *pData, uint32 dwAudioDataSize, uint32 dwDataCursor) {
    if (!pData) {
        return KS_ERR_INVALID_PARAMETER;
    }

    uint32 totalBlockCount = _hcaInfo.blockCount;
    uint32 processedDataSize = 0;
    uint32 blockSize = _hcaInfo.blockSize;
    uint8 *dataCursor = pData + dwDataCursor;
    dwAudioDataSize -= dwDataCursor;

    KS_RESULT result;
    for (uint32 i = 0; i < totalBlockCount; ++i) {
        if (processedDataSize >= dwAudioDataSize) {
            return KS_ERR_BUFFER_TOO_SMALL;
        }
        result = ConvertBlock(dataCursor, blockSize);
        if (!KS_CALL_SUCCESSFUL(result)) {
            return result;
        }
        processedDataSize += blockSize;
        dataCursor += blockSize;
    }

    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::ConvertBlock(uint8 *pData, uint32 dwBlockSize) {
    if (!pData) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (dwBlockSize != _hcaInfo.blockSize) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (Checksum(pData, dwBlockSize, 0) != 0) {
        return KS_ERR_CHECKSUM_NOT_MATCH;
    }
    _cipherFrom.Decrypt(pData, dwBlockSize - 2);

    CHcaData data(pData, dwBlockSize);
    int32 magic = data.GetBit(16);
    if (magic != 0xffff) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    _cipherTo.Encrypt(pData, dwBlockSize - 2);
    auto result = FixBlock(pData, dwBlockSize);
    if (!KS_CALL_SUCCESSFUL(result)) {
        return result;
    }
    return KS_ERR_OK;
}

KS_RESULT CHcaDecoder::FixBlock(uint8 *pData, uint32 dwBlockSize) {
    if (!pData) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (dwBlockSize != _hcaInfo.blockSize) {
        return KS_ERR_INVALID_PARAMETER;
    }
    uint16 wSum = Checksum(pData, dwBlockSize - 2, 0);
    *(uint16 *)(pData + dwBlockSize - 2) = bswap(wSum);
    return KS_ERR_OK;
}

KS_RESULT
CHcaDecoder::SetNewCipherType(uint8 *pData, uint32 dwFileSize, HCA_CIPHER_TYPE cipherType, uint32 dwDataOffset) {
    if (!pData) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (dwFileSize < sizeof(HCA_HEADER)) {
        return KS_ERR_INVALID_PARAMETER;
    }
    uint32 cursor = 0;

    // Check HCA header (of the whole file).
    HCA_HEADER *hca = (HCA_HEADER *)(pData + cursor);
    if (!Magic::Match(hca->hca, Magic::HCA)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    // The file must be bigger than a raw HCA header.
    uint32 dataOffset = bswap(hca->dataOffset);
    if (dwFileSize < dataOffset) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (Checksum(hca, dataOffset, 0) != 0) {
        // File may be broken.
        return KS_ERR_CHECKSUM_NOT_MATCH;
    }
    cursor += sizeof(HCA_HEADER);

    // FMT
    HCA_FORMAT *fmt = (HCA_FORMAT *)(pData + cursor);
    if (!Magic::Match(fmt->fmt, Magic::FORMAT)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    cursor += sizeof(HCA_FORMAT);

    // COMP or DEC
    if (Magic::Match(*((uint32 *)(pData + cursor)), Magic::COMPRESS)) {
        cursor += sizeof(HCA_COMPRESS);
    } else if (Magic::Match(*((uint32 *)(pData + cursor)), Magic::DECODE)) {
        cursor += sizeof(HCA_DECODE);
    } else {
        return KS_ERR_MAGIC_NOT_MATCH;
    }

    // VBR
    HCA_VBR *vbr = (HCA_VBR *)(pData + cursor);
    if (Magic::Match(vbr->vbr, Magic::VBR)) {
        cursor += sizeof(HCA_VBR);
    }

    // ATH
    HCA_ATH *ath = (HCA_ATH *)(pData + cursor);
    if (Magic::Match(ath->ath, Magic::ATH)) {
        cursor += sizeof(HCA_ATH);
    }

    // LOOP
    HCA_LOOP *loop = (HCA_LOOP *)(pData + cursor);
    if (Magic::Match(loop->loop, Magic::LOOP)) {
        cursor += sizeof(HCA_LOOP);
    }

    // CIPH
    HCA_CIPHER *ciph = (HCA_CIPHER *)(pData + cursor);
    if (Magic::Match(ciph->ciph, Magic::CIPHER)) {
        uint16 ct = cipherType;
        ct = bswap(ct);
        ciph->type = ct;
    }

    return FixHeader(pData, dwFileSize, dwDataOffset);
}

KS_RESULT CHcaDecoder::FixHeader(uint8 *pData, uint32 dwFileSize, uint32 dwDataOffset) {
    if (dwFileSize < dwDataOffset) {
        return KS_ERR_INVALID_PARAMETER;
    }
    // Recalculate the checksum and write to the last WORD before dwDataOffset.
    uint16 wSum = Checksum(pData, dwDataOffset - 2, 0);
    *(uint16 *)(pData + dwDataOffset - 2) = bswap(wSum);
    return KS_ERR_OK;
}
