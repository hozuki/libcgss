#include <stdlib.h>
#include "../../common/cgss_building_dll.h"
#include "../api/ks_decode_helper.h"
#include "../../ks_api.h"
#include "ks_ext.h"
#include "../../takamori/streams/tm_memory_stream.h"

#ifndef __MINGW_H

#include <algorithm>

#endif

using namespace std;

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

uint32 KsExtStreamingGetMappedPosition(KS_DECODE *hDecode, uint32 requestedPosition);

KS_RESULT KsExtStreamingEnsureDecoded(KS_DECODE *hDecode, uint32 offset, uint32 count);

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
    auto memoryStream = streamingStatus->memoryStream;
    auto positionInWaveStream = KsExtStreamingGetMappedPosition(decode, (uint32)memoryStream->GetPosition());
    auto headerCrossData = FALSE;
    uint32 origPosInStream = 0;
    auto waveHeaderSize = KsGetWaveHeaderDataSize(decode);

    if (positionInWaveStream + dwBufferSize < waveHeaderSize) {
        read = (uint32)memoryStream->Read(pBuffer, dwBufferSize, 0, dwBufferSize);
        if (pdwBytesRead) {
            *pdwBytesRead = read;
        }
        return memoryStream->GetPosition() < memoryStream->GetLength() ? KS_OP_HAS_MORE_DATA : KS_ERR_OK;
    } else {
        if (positionInWaveStream < waveHeaderSize) {
            origPosInStream = positionInWaveStream;
            positionInWaveStream = waveHeaderSize;
            headerCrossData = TRUE;
        }
    }
    auto r = KsExtStreamingEnsureDecoded(decode, (uint32)memoryStream->GetPosition(), dwBufferSize);
    if (!KS_CALL_SUCCESSFUL(r)) {
        if (pdwBytesRead) {
            *pdwBytesRead = 0;
        }
        return r;
    }
    if (headerCrossData) {
        positionInWaveStream = origPosInStream;
    }
    memoryStream->SetPosition(positionInWaveStream);
    HCA_INFO hcaInfo;
    decode->hca->GetInfo(hcaInfo);
    if (hcaInfo.loopExists) {
        auto waveBlockSize = KsGetWaveBlockDataSize(decode);
        auto endLoopDataPositionIncludingHeader = hcaInfo.loopEnd * waveBlockSize + waveHeaderSize;
        auto shouldRead = min(dwBufferSize, endLoopDataPositionIncludingHeader - positionInWaveStream);
        read = (uint32)memoryStream->Read(pBuffer, dwBufferSize, 0, shouldRead);
    } else {
        read = (uint32)memoryStream->Read(pBuffer, dwBufferSize, 0, dwBufferSize);
    }
    if (pdwBytesRead) {
        *pdwBytesRead = read;
    }
    return memoryStream->GetPosition() < memoryStream->GetLength() ? KS_OP_HAS_MORE_DATA : KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsExtStreamingSeek(KS_DECODE_HANDLE hDecode, uint32 dwPosition) {
    GENERAL_CHECK();
    auto streamingStatus = decode->extStreamingStatus;
    auto position = min(dwPosition, (uint32)streamingStatus->memoryStream->GetLength());
    streamingStatus->memoryStream->SetPosition(position);
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsExtStreamingTell(KS_DECODE_HANDLE hDecode, uint32 *pdwPosition) {
    if (!pdwPosition) {
        return KS_ERR_INVALID_PARAMETER;
    }
    GENERAL_CHECK();
    *pdwPosition = (uint32)decode->extStreamingStatus->memoryStream->GetPosition();
    return KS_ERR_OK;
}

KS_RESULT KsExtStreamingEnsureDecoded(KS_DECODE *hDecode, uint32 offset, uint32 count) {
    auto &decodeStatus = hDecode->status;
    auto streamingStatus = hDecode->extStreamingStatus;
    auto notFound = streamingStatus->decodedBlocks.end();
    auto waveHeaderSize = KsGetWaveHeaderDataSize(hDecode);
    auto waveBlockSize = KsGetWaveBlockDataSize(hDecode);
    auto startBlockIndex = (offset - waveHeaderSize) / waveBlockSize;
    auto endBlockIndex = (offset + count - waveHeaderSize) / waveBlockSize;
    HCA_INFO hcaInfo;
    hDecode->hca->GetInfo(hcaInfo);
    endBlockIndex = min(endBlockIndex, hcaInfo.blockCount - 1);
    auto decodeBuffer = new uint8[waveBlockSize];
    KS_RESULT result = KS_ERR_OK;
    for (auto i = startBlockIndex; i <= endBlockIndex; ++i) {
        if (streamingStatus->decodedBlocks.find(i) != notFound) {
            continue;
        }
        decodeStatus.blockIndex = i;
        decodeStatus.dataCursor = i * hcaInfo.blockSize + hcaInfo.dataOffset;
        decodeStatus.loopNumber = 0;
        uint32 decodeBufferSize = waveBlockSize;
        result = KsDecodeData(hDecode, decodeBuffer, &decodeBufferSize);
        if (!KS_CALL_SUCCESSFUL(result)) {
            break;
        }
        auto positionInOutputStream = waveHeaderSize + i * waveBlockSize;
        memcpy(streamingStatus->memoryStream->GetBuffer() + positionInOutputStream, decodeBuffer, decodeBufferSize);
        streamingStatus->decodedBlocks.insert(i);
    }
    delete[] decodeBuffer;
    return result;
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
    streamingStatus->memoryStream = new MemoryStream(waveTotalSize, FALSE);
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
    delete[] hDecode->extStreamingStatus->memoryStream;
    hDecode->extStreamingStatus->memoryStream = nullptr;
    hDecode->extStreamingStatus->decodedBlocks.clear();
    delete hDecode->extStreamingStatus;
    hDecode->extStreamingStatus = nullptr;
    return KS_ERR_OK;
}
