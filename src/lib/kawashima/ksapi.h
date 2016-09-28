#ifndef KAWASHIMA_KSAPI_H
#define KAWASHIMA_KSAPI_H

#define BUILDING_DLL

#include "kstypedef.h"
#include "hca_info.h"

#ifdef _MSC_VER
#define DECL_KS_API(ret_type, name, params) EXTERN_C DLL_PUBLIC ret_type STDCALL name params
#else
#define DECL_KS_API(ret_type, name, params) EXTERN_C DLL_PUBLIC STDCALL ret_type name params
#endif

typedef void *HKDECODE;

typedef enum {
	KS_PARAM_BUFFER_SIZE,
	KS_PARAM_KEY1,
	KS_PARAM_KEY2,
	KS_PARAM_KEY,
	KS_PARAM_FORCE_DWORD = 0xffffffff
} KS_PARAM_TYPE;

typedef enum {
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
	KS_ERR_FORCE_DWORD = 0xffffffff
} KS_RESULT;

#define KS_CALL_SUCCESSFUL(x) ((ubool)(((int32)x) >= 0))

DECL_KS_API(KS_RESULT, KsOpenFile, (const char *pFileName, HKDECODE *ppHandle));
DECL_KS_API(KS_RESULT, KsOpenBuffer, (uint8 *pData, uint32 dwDataSize, ubool bClone, HKDECODE *ppHandle));
DECL_KS_API(KS_RESULT, KsSetParamI32, (HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint32 dwParam));
DECL_KS_API(KS_RESULT, KsSetParamI64, (HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint64 qwParam));
DECL_KS_API(KS_RESULT, KsBeginDecode, (HKDECODE hDecode));
DECL_KS_API(KS_RESULT, KsGetWaveHeader, (HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_KS_API(KS_RESULT, KsDecodeData, (HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize));
DECL_KS_API(KS_RESULT, KsEndDecode, (HKDECODE hDecode));
DECL_KS_API(KS_RESULT, KsCloseHandle, (HKDECODE hDecode));
DECL_KS_API(KS_RESULT, KsGetHcaInfo, (HKDECODE hDecode, HCA_INFO *pInfo));
DECL_KS_API(ubool, KsIsActiveHandle, (HKDECODE hDecode));
DECL_KS_API(ubool, KsIsHcaCheckPassed, (HKDECODE hDecode));
DECL_KS_API(KS_RESULT, KsHasMoreData, (HKDECODE hDecode, ubool *pbHasMore));
DECL_KS_API(void, KsTest, ());

#undef DECL_KS_API

#endif //KAWASHIMA_API_H
