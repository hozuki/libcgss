#ifndef KAWASHIMA_HCA_H
#define KAWASHIMA_HCA_H

#include "../../cgss_typedef.h"
#include "../../hca_info.hpp"
#include "../api/ks_decode.h"
#include "../../ks_api.h"
#include "internal/CHcaCipher.h"
#include "internal/CHcaAth.h"
#include "internal/HcaChannel.h"

struct _KS_DECODE_STATUS;
typedef struct _KS_DECODE_STATUS KS_DECODE_STATUS;

class CHcaDecoder {

public:

    typedef void (*DecodeFunc)(float data, uint8 *buffer, uint32 *cursor);

    CHcaDecoder();

    CHcaDecoder(const HCA_CIPHER_CONFIG &crypt);

    CHcaDecoder(const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo);

    KS_RESULT ReadHeader(uint8 *pData, uint32 dwFileSize, uint32 *pdwDataOffset);

    KS_RESULT ReadHeader(uint8 *pFileData, uint32 dwDataSize, KS_DECODE_STATUS *pStatus);

    KS_RESULT SetNewCipherType(uint8 *pData, uint32 dwFileSize, HCA_CIPHER_TYPE cipherType, uint32 dwDataOffset);

    KS_RESULT ConvertData(uint8 *pData, uint32 dwAudioDataSize, uint32 dwDataCursor);

    KS_RESULT GetWaveHeader(uint8 *pBuffer, uint32 *pdwWaveHeaderSize);

    KS_RESULT DecodeData(uint8 *pData, uint32 dwDataSize, KS_DECODE_STATUS *status, uint8 *pWaveData,
                         uint32 *pdwWaveDataSize, ubool *pbHasMore);

    KS_RESULT GenerateWaveDataBlock(uint8 *pData, uint32 dwBlockSize, uint32 *pDataCursor, uint8 *pBuffer,
                                    uint32 *pBufferCursor, DecodeFunc pfnDecodeFunc);

    KS_RESULT DecodeBlock(uint8 *pData, uint32 dwBlockSize, uint32 *pDataCursor);

    KS_RESULT GetInfo(HCA_INFO *pInfo) const;

    KS_RESULT GetInfo(HCA_INFO &info) const;

private:

    static uint16 Checksum(void *pData, uint32 dwDataSize, uint16 wInitSum);

    KS_RESULT ConvertBlock(uint8 *pData, uint32 dwBlockSize);

    KS_RESULT FixBlock(uint8 *pData, uint32 dwBlockSize);

    KS_RESULT FixHeader(uint8 *pData, uint32 dwFileSize, uint32 dwDataOffset);

    CHcaAth _ath;
    CHcaCipher _cipherFrom, _cipherTo;
    HCA_CIPHER_CONFIG _ccFrom, _ccTo;
    HcaChannel _channels[0x10];
    HCA_INFO _hcaInfo;

    static const uint16 *ChecksumTable;

};

#endif
