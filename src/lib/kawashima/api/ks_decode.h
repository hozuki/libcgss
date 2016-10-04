#ifndef KAWASHIMA_KS_DECODE_H
#define KAWASHIMA_KS_DECODE_H

#include <set>
#include "../../cgss_typedef.h"
#include "../../hca_info.hpp"
#include "../../ks_api.h"
#include "../hca/CHcaDecoder.h"

typedef enum _KS_DECODE_STAGE {
    KS_STAGE_INVALID = 0,
    KS_STAGE_INITIALIZED = 1,
    KS_STAGE_DECODE_STARTED = 2,
    KS_STAGE_DATA_DECODING = 3,
    KS_STAGE_DECODING_COMPLETE = 4,
    KS_STAGE_FORCE_DWORD = (uint32)0xffffffff
} KS_DECODE_STAGE;

typedef struct _KS_DECODE_PARAM {
    uint32 key1;
    uint32 key2;
    // Unused
    uint32 bufferSize;
} KS_DECODE_PARAM;

typedef struct _KS_DECODE_STATUS {
    KS_DECODE_STAGE stage;
    ubool hcaChecked;
    ubool hcaCheckFailed;
    uint32 dataCursor;
    uint32 blockIndex;
    uint32 loopNumber;
} KS_DECODE_STATUS;

class CHcaDecoder;

typedef struct _KS_EXT_STREAMING_STATUS {
    uint8 *streamingData;
    uint32 streamingDataSize;
    uint32 cursorPosition;
    std::set<uint32> decodedBlocks;
} KS_EXT_STREAMING_STATUS;

typedef struct _KS_DECODE {
    uint32 cb;
    uint32 magic;
    ubool closed;
    CHcaDecoder *hca;
    uint8 *data;
    uint32 dataSize;
    ubool dataIsCopy;
    KS_DECODE_STATUS status;
    KS_DECODE_PARAM params;
    // v1.1
    KS_DECODE_EXTENSION availableExtensions;
    KS_DECODE_EXTENSION enabledExtensions;
    ubool extensionsPrepared;
    KS_EXT_STREAMING_STATUS *extStreamingStatus;
} KS_DECODE;

#endif //KAWASHIMA_KS_DECODE_H
