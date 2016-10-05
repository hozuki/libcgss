#include <stdlib.h>
#include "../../common/cgss_building_dll.h"
#include "../api/ks_decode_helper.h"
#include "../../ks_api.h"
#include "ks_ext.h"

#ifndef __MINGW_H

#include <algorithm>

#endif

#define GENERAL_CHECK() \
            KS_DECODE *decode = (KS_DECODE *)hDecode; \
            do { \
                if (!hDecode) { \
                    return KS_ERR_INVALID_HANDLE; \
                } \
                if (!KsCheckMagic(decode)) { \
                    return KS_ERR_MAGIC_NOT_MATCH; \
                } \
                if (decode->closed) { \
                    return KS_ERR_ALREADY_CLOSED; \
                } \
                if (decode->status.stage < KS_STAGE_DECODE_STARTED) { \
                    return KS_ERR_INVALID_OPERATION; \
                } \
                if (!KsIsExtensionAvailable(decode, KS_EXTENSION_STREAMING)) { \
                    return KS_ERR_EXTENSION_NOT_AVAILABLE; \
                } \
                if (!KsIsExtensionEnabled(decode, KS_EXTENSION_STREAMING)) { \
                    return KS_ERR_EXTENSION_NOT_ENABLED;\
                } \
            } while (0)

inline uint32 safe_read(uint8 *destination, uint8 *src, uint32 destination_len, uint32 src_max_len) {
    auto len = std::min(destination_len, src_max_len);
    memcpy(destination, src, len);
    return len;
}

uint32 KsExtStreamingGetMappedPosition(KS_DECODE *hDecode, uint32 requestedPosition);

void KsExtStreamingEnsureDecoded(KS_DECODE *hDecode, uint32 offset, uint32 count);

CGSS_API_TYPE(KS_RESULT) KsExtStreamingGetSize(KS_DECODE_HANDLE hDecode, uint32 *pdwSizeInBytes) {
    if (!pdwSizeInBytes) {
        return KS_ERR_INVALID_PARAMETER;
    }
    GENERAL_CHECK();
    return KsGetFullWaveSize(hDecode, TRUE, pdwSizeInBytes);
}

CGSS_API_TYPE(KS_RESULT) KsExtStreamingRead(KS_DECODE_HANDLE hDecode, uint8 *pBuffer, uint32 dwBufferSize, uint32 *pdwBytesRead) {
    if (!pBuffer) {
        return KS_ERR_INVALID_PARAMETER;
    }
    GENERAL_CHECK();
    uint32 read;
    ubool hasMoreData;
    KsHasMoreData(hDecode, &hasMoreData);
    if (!hasMoreData) {
        if (pdwBytesRead) {
            *pdwBytesRead = 0;
        }
        return KS_ERR_OK;
    }
    auto &decodeStatus = decode->status;
    auto streamingStatus = decode->extStreamingStatus;
    auto positionInWaveStream = KsExtStreamingGetMappedPosition(decode, streamingStatus->cursorPosition);
    auto headerCrossData = FALSE;
    uint32 origPosInStream = 0;
    auto waveHeaderSize = KsGetWaveHeaderDataSize(decode);
    auto waveBlockSize = KsGetWaveBlockDataSize(decode);

    if (positionInWaveStream + dwBufferSize < waveHeaderSize) {
        read = safe_read(pBuffer, streamingStatus->streamingData + streamingStatus->cursorPosition, dwBufferSize,
                         streamingStatus->streamingDataSize - streamingStatus->cursorPosition);
        streamingStatus->cursorPosition += read;
        if (pdwBytesRead) {
            *pdwBytesRead = read;
        }
        return streamingStatus->cursorPosition < streamingStatus->streamingDataSize ? KS_OP_HAS_MORE_DATA : KS_ERR_OK;
    } else {
        if (positionInWaveStream < waveHeaderSize) {
            origPosInStream = positionInWaveStream;
            positionInWaveStream = waveHeaderSize;
            headerCrossData = TRUE;
        }
    }
    KsExtStreamingEnsureDecoded(decode, streamingStatus->cursorPosition, dwBufferSize);
    if (headerCrossData) {
        positionInWaveStream = origPosInStream;
    }
    streamingStatus->cursorPosition = positionInWaveStream;
    HCA_INFO hcaInfo;
    decode->hca->GetInfo(hcaInfo);
    if (hcaInfo.loopExists) {
        auto endLoopDataPositionIncludingHeader = hcaInfo.loopEnd * waveBlockSize + waveHeaderSize;
        auto shouldRead = std::min(dwBufferSize, endLoopDataPositionIncludingHeader - positionInWaveStream);
        read = safe_read(pBuffer, streamingStatus->streamingData + streamingStatus->cursorPosition, shouldRead,
                         streamingStatus->streamingDataSize - streamingStatus->cursorPosition);
    } else {
        read = safe_read(pBuffer, streamingStatus->streamingData + streamingStatus->cursorPosition, dwBufferSize,
                         streamingStatus->streamingDataSize - streamingStatus->cursorPosition);
    }
    streamingStatus->cursorPosition += read;
    if (pdwBytesRead) {
        *pdwBytesRead = read;
    }
    return streamingStatus->cursorPosition < streamingStatus->streamingDataSize ? KS_OP_HAS_MORE_DATA : KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsExtStreamingSeek(KS_DECODE_HANDLE hDecode, uint32 dwPosition) {
    GENERAL_CHECK();
    auto streamingStatus = decode->extStreamingStatus;
    auto position = std::min(dwPosition, streamingStatus->streamingDataSize);
    streamingStatus->cursorPosition = position;
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsExtStreamingTell(KS_DECODE_HANDLE hDecode, uint32 *pdwPosition) {
    if (!pdwPosition) {
        return KS_ERR_INVALID_PARAMETER;
    }
    GENERAL_CHECK();
    *pdwPosition = decode->extStreamingStatus->cursorPosition;
    return KS_ERR_OK;
}

void KsExtStreamingEnsureDecoded(KS_DECODE *hDecode, uint32 offset, uint32 count) {
    auto &decodeStatus = hDecode->status;
    auto streamingStatus = hDecode->extStreamingStatus;
    auto null = streamingStatus->decodedBlocks.end();
    auto waveHeaderSize = KsGetWaveHeaderDataSize(hDecode);
    auto waveBlockSize = KsGetWaveBlockDataSize(hDecode);
    auto startBlockIndex = (offset - waveHeaderSize) / waveBlockSize;
    auto endBlockIndex = (offset + count - waveHeaderSize) / waveBlockSize;
    HCA_INFO hcaInfo;
    hDecode->hca->GetInfo(hcaInfo);
    endBlockIndex = std::min(endBlockIndex, hcaInfo.blockCount - 1);
    auto decodeBuffer = new uint8[waveBlockSize];
    auto streamingData = streamingStatus->streamingData;
    for (auto i = startBlockIndex; i <= endBlockIndex; ++i) {
        if (streamingStatus->decodedBlocks.find(i) != null) {
            continue;
        }
        decodeStatus.blockIndex = i;
        decodeStatus.dataCursor = i * hcaInfo.blockSize + hcaInfo.dataOffset;
        decodeStatus.loopNumber = 0;
        uint32 decodeBufferSize = waveBlockSize;
        auto r = KsDecodeData(hDecode, decodeBuffer, &decodeBufferSize);
        auto positionInOutputStream = waveHeaderSize + i * waveBlockSize;
        memcpy(streamingData + positionInOutputStream, decodeBuffer, decodeBufferSize);
        streamingStatus->decodedBlocks.insert(i);
    }
    delete[] decodeBuffer;
}

// Actually the HCA should be decoded to a finite length wave stream. If there are loops,
// we handle them here. For example, there are 4 loops and we are reading the virtual
// wave stream, which maps to the 3rd loop of original wave. Here is what this function do.
uint32 KsExtStreamingGetMappedPosition(KS_DECODE *hDecode, uint32 requestedPosition) {
    auto position = requestedPosition;
    HCA_INFO hcaInfo;
    hDecode->hca->GetInfo(hcaInfo);
    if (!hcaInfo.loopExists) {
        return position;
    }
    auto waveHeaderSize = KsGetWaveHeaderDataSize(hDecode);
    auto waveBlockSize = KsGetWaveBlockDataSize(hDecode);
    auto relativePosition = position - waveHeaderSize;
    auto endLoopDataPosition = hcaInfo.loopEnd * waveBlockSize;
    if (relativePosition < endLoopDataPosition) {
        return position;
    }
    auto startLoopDataPosition = hcaInfo.loopStart * waveBlockSize;
    auto waveDataLength = endLoopDataPosition - startLoopDataPosition;
    auto positionInsideLoop = (relativePosition - startLoopDataPosition) % waveDataLength;
    position = waveHeaderSize + startLoopDataPosition + positionInsideLoop;
    return position;
}

KS_RESULT KsExtensionStreamingInitializer(KS_DECODE *hDecode) {
    ubool isExtensionAvailable;
    auto r = KsIsExtensionAvailable(hDecode, KS_EXTENSION_STREAMING, &isExtensionAvailable);
    if (!KS_CALL_SUCCESSFUL(r) || !isExtensionAvailable) {
        return KS_ERR_EXTENSION_NOT_AVAILABLE;
    }
    uint32 waveTotalSize;
    r = KsGetFullWaveSize(hDecode, TRUE, &waveTotalSize);
    if (!KS_CALL_SUCCESSFUL(r)) {
        return KS_ERR_INVALID_INTERNAL_STATE;
    }
    auto streamingStatus = new KS_EXT_STREAMING_STATUS();
    streamingStatus->cursorPosition = 0;
    streamingStatus->streamingDataSize = waveTotalSize;
    streamingStatus->streamingData = new uint8[waveTotalSize];
    hDecode->extStreamingStatus = streamingStatus;
    return KS_ERR_OK;
}

KS_RESULT KsExtensionStreamingFinalizer(KS_DECODE *hDecode) {
    ubool b;
    auto r = KsIsExtensionAvailable(hDecode, KS_EXTENSION_STREAMING, &b);
    if (!KS_CALL_SUCCESSFUL(r) || !b) {
        return KS_ERR_EXTENSION_NOT_AVAILABLE;
    }
    r = KsIsExtensionEnabled(hDecode, KS_EXTENSION_STREAMING, &b);
    if (!KS_CALL_SUCCESSFUL(r) || !b) {
        return KS_ERR_EXTENSION_NOT_ENABLED;
    }
    if (!hDecode->extStreamingStatus) {
        return KS_ERR_INVALID_PARAMETER;
    }
    delete[] hDecode->extStreamingStatus->streamingData;
    hDecode->extStreamingStatus->streamingData = nullptr;
    hDecode->extStreamingStatus->decodedBlocks.clear();
    delete hDecode->extStreamingStatus;
    hDecode->extStreamingStatus = nullptr;
    return KS_ERR_OK;
}
