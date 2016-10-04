#ifndef KAWASHIMA_KSAPI_H
#define KAWASHIMA_KSAPI_H

#include "cgss_typedef.h"
#include "hca_info.hpp"

typedef void *HKDECODE;

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
    KS_ERR_FEATURE_UNAVAILABLE = -18,
    KS_ERR_FEATURE_NOT_ENABLED = -19,
    KS_ERR_FORCE_DWORD = 0xffffffff
} KS_RESULT;

#define KS_CALL_SUCCESSFUL(x) ((ubool)(((int32)x) >= 0))

DECL_CGSS_API(KS_RESULT, KsOpenFile, (const char *pFileName, HKDECODE *ppHandle));
DECL_CGSS_API(KS_RESULT, KsOpenBuffer, (uint8 *pData, uint32 dwDataSize, ubool bClone, HKDECODE *ppHandle));
DECL_CGSS_API(KS_RESULT, KsSetParamI32, (HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint32 dwParam));
DECL_CGSS_API(KS_RESULT, KsSetParamI64, (HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint64 qwParam));
DECL_CGSS_API(KS_RESULT, KsBeginDecode, (HKDECODE hDecode));
DECL_CGSS_API(KS_RESULT, KsGetWaveHeader, (HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_CGSS_API(KS_RESULT, KsDecodeData, (HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_CGSS_API(KS_RESULT, KsEndDecode, (HKDECODE hDecode));
DECL_CGSS_API(KS_RESULT, KsCloseHandle, (HKDECODE hDecode));
DECL_CGSS_API(KS_RESULT, KsGetHcaInfo, (HKDECODE hDecode, HCA_INFO *pInfo));
DECL_CGSS_API(ubool, KsIsActiveHandle, (HKDECODE hDecode));
DECL_CGSS_API(ubool, KsIsHcaCheckPassed, (HKDECODE hDecode));
DECL_CGSS_API(KS_RESULT, KsHasMoreData, (HKDECODE hDecode, ubool *pbHasMore));
DECL_CGSS_API(void, KsTest, ());
// v1.1
DECL_CGSS_API(KS_RESULT, KsGetWaveSize, (HKDECODE hDecode, ubool bWithHeader, uint32 *pdwSize));
DECL_CGSS_API(KS_RESULT, KsDecodeAllData, (HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_CGSS_API(KS_RESULT, KsEnableFeature, (HKDECODE hDecode, HCA_DECODE_FEATURE dwFeature, ubool bEnabled));
DECL_CGSS_API(KS_RESULT, KsIsFeatureEnabled, (HKDECODE hDecode, HCA_DECODE_FEATURE dwFeature, ubool *pbIsEnabled));
DECL_CGSS_API(KS_RESULT, KsFeatStreamingGetSize, (HKDECODE hDecode,uint32 *pdwSizeInBytes));
DECL_CGSS_API(KS_RESULT, KsFeatStreamingRead, (HKDECODE hDecode, uint8 *pBuffer, uint32 dwBufferSize, uint32 *pdwBytesRead));
DECL_CGSS_API(KS_RESULT, KsFeatStreamingSeek, (HKDECODE hDecode, uint32 dwPosition));
DECL_CGSS_API(KS_RESULT, KsFeatStreamingTell, (HKDECODE hDecode, uint32 *pdwPosition));

#endif //KAWASHIMA_API_H
