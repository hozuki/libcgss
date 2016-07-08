#ifndef KAWASHIMA_HCA_H
#define KAWASHIMA_HCA_H

#include "kstypedef.h"
#include "hca_info.h"
#include "internal/CHcaCipher.h"

class CHCA {

public:

    typedef void (*DecodeFunc)(float data, uint8 *buffer, uint32 *cursor);

    CHCA(HCA_CIPHER_CONFIG &cryptFrom, HCA_CIPHER_CONFIG &cryptTo);

    KS_RESULT ReadHeader(uint8 *pData, uint32 dwFileSize, uint32 *pdwDataOffset);

    KS_RESULT SetNewCipherType(uint8 *pData, uint32 dwFileSize, HCA_CIPHER_TYPE cipherType, uint32 dwDataOffset);

    KS_RESULT ConvertData(uint8 *pData, uint32 dwAudioDataSize, uint32 dwDataCursor);

private:

    static uint16 Checksum(void *pData, uint32 dwDataSize, uint16 wInitSum);

    KS_RESULT ConvertBlock(uint8 *pData, uint32 dwBlockSize);

    KS_RESULT FixBlock(uint8 *pData, uint32 dwBlockSize);

    KS_RESULT FixHeader(uint8 *pData, uint32 dwFileSize, uint32 dwDataOffset);

    CHcaCipher _cipherFrom, _cipherTo;
    HCA_CIPHER_CONFIG _ccFrom, _ccTo;
    HCA_INFO _hcaInfo;

    static const uint16 *ChecksumTable;

};

#endif
