#ifndef KAWASHIMA_KSAPI_H
#define KAWASHIMA_KSAPI_H

#include "cgss_typedef.h"
#include "hca_info.hpp"

typedef void *KS_DECODE_HANDLE;

typedef enum _KS_DECODE_EXTENSION {
    KS_EXTENSION_NONE = 0,
    KS_EXTENSION_STREAMING = 1,
    KS_EXTENSION_FORCE_DWORD = 0xffffffff
} KS_DECODE_EXTENSION;

typedef enum _KS_PARAM_TYPE {
    KS_PARAM_BUFFER_SIZE,
    KS_PARAM_KEY1,
    KS_PARAM_KEY2,
    KS_PARAM_KEY,
    KS_PARAM_FORCE_DWORD = 0xffffffff
} KS_PARAM_TYPE;

typedef enum _KS_RESULT {
    KS_OP_HAS_MORE_DATA = 1,
    KS_ERR_OK = 0,
    KS_ERR_INVALID_HANDLE = -1,
    KS_ERR_MAGIC_NOT_MATCH = -2,
    KS_ERR_ALREADY_CLOSED = -3,
    KS_ERR_INVALID_PARAMETER = -4,
    KS_ERR_INVALID_STAGE = -5,
    KS_ERR_FILE_OP_FAILED = -6,
    KS_ERR_INVALID_FILE_PROP = -7,
    KS_ERR_INVALID_OPERATION = -8,
    KS_ERR_DECODE_FAILED = -9,
    KS_ERR_BUFFER_TOO_SMALL = -10,
    KS_ERR_CHECKSUM_NOT_MATCH = -11,
    KS_ERR_INVALID_INTERNAL_STATE = -12,
    KS_ERR_ATH_INIT_FAILED = -13,
    KS_ERR_CIPH_INIT_FAILED = -14,
    KS_ERR_STATE_OUT_OF_RANGE = -15,
    KS_ERR_NOT_IMPLEMENTED = -16,
    KS_ERR_DECODE_ALREADY_COMPLETED = -17,
    KS_ERR_EXTENSION_NOT_AVAILABLE = -18,
    KS_ERR_EXTENSION_NOT_ENABLED = -19,
    KS_ERR_FORCE_DWORD = 0xffffffff
} KS_RESULT;

#define KS_CALL_SUCCESSFUL(x) ((ubool)(((int32)x) >= 0))

DECL_CGSS_API(KS_RESULT, KsOpenFile, (const char *pFileName, KS_DECODE_HANDLE *ppHandle));
DECL_CGSS_API(KS_RESULT, KsOpenBuffer, (uint8 *pData, uint32 dwDataSize, ubool bClone, KS_DECODE_HANDLE *ppHandle));
DECL_CGSS_API(KS_RESULT, KsSetParamI32, (KS_DECODE_HANDLE hDecode, KS_PARAM_TYPE dwParamType, uint32 dwParam));
DECL_CGSS_API(KS_RESULT, KsSetParamI64, (KS_DECODE_HANDLE hDecode, KS_PARAM_TYPE dwParamType, uint64 qwParam));
DECL_CGSS_API(KS_RESULT, KsBeginDecode, (KS_DECODE_HANDLE hDecode));
DECL_CGSS_API(KS_RESULT, KsGetWaveHeader, (KS_DECODE_HANDLE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_CGSS_API(KS_RESULT, KsDecodeData, (KS_DECODE_HANDLE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_CGSS_API(KS_RESULT, KsEndDecode, (KS_DECODE_HANDLE hDecode));
DECL_CGSS_API(KS_RESULT, KsCloseHandle, (KS_DECODE_HANDLE hDecode));
DECL_CGSS_API(KS_RESULT, KsGetHcaInfo, (KS_DECODE_HANDLE hDecode, HCA_INFO *pInfo));
DECL_CGSS_API(ubool, KsIsActiveHandle, (KS_DECODE_HANDLE hDecode));
DECL_CGSS_API(ubool, KsIsHcaCheckPassed, (KS_DECODE_HANDLE hDecode));
DECL_CGSS_API(KS_RESULT, KsHasMoreData, (KS_DECODE_HANDLE hDecode, ubool *pbHasMore));
DECL_CGSS_API(void, KsTest, ());
// v1.1
DECL_CGSS_API(KS_RESULT, KsGetFullWaveSize, (KS_DECODE_HANDLE hDecode, ubool bWithHeader, uint32 * pdwSize));
DECL_CGSS_API(KS_RESULT, KsDecodeAllData, (KS_DECODE_HANDLE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_CGSS_API(KS_RESULT, KsPrepareExtensions, (KS_DECODE_HANDLE hDecode));
DECL_CGSS_API(KS_RESULT, KsGetAvailableExtensions, (KS_DECODE_HANDLE *hDecode, KS_DECODE_EXTENSION *pdwExtensions));
DECL_CGSS_API(KS_RESULT, KsEnableExtension, (KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION dwExtension, ubool bEnabled));
DECL_CGSS_API(KS_RESULT, KsIsExtensionEnabled, (KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION dwExtension, ubool *pbIsEnabled));
DECL_CGSS_API(KS_RESULT, KsIsExtensionAvailable, (KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION dwExtension, ubool *pbIsAvailable));
DECL_CGSS_API(KS_RESULT, KsExtStreamingGetSize, (KS_DECODE_HANDLE hDecode, uint32 * pdwSizeInBytes));
DECL_CGSS_API(KS_RESULT, KsExtStreamingRead, (KS_DECODE_HANDLE hDecode, uint8 *pBuffer, uint32 dwBufferSize, uint32 *pdwBytesRead));
DECL_CGSS_API(KS_RESULT, KsExtStreamingSeek, (KS_DECODE_HANDLE hDecode, uint32 dwPosition));
DECL_CGSS_API(KS_RESULT, KsExtStreamingTell, (KS_DECODE_HANDLE hDecode, uint32 *pdwPosition));

#endif //KAWASHIMA_API_H
