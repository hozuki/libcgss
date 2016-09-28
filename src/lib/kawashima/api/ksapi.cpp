/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifdef __CYGWIN__

// Force using MBCS rather than Unicode.
#define _MBCS

#include <windows.h>
#include <iostream>

#else

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream>

#endif

#include "../ksapi.h"
#include "ks_decode.h"

static const uint32 KS_DECODE_MAGIC = 0x491c39a6;

inline ubool check_magic(KS_DECODE *hDecode) {
    return hDecode->magic == KS_DECODE_MAGIC ? TRUE : FALSE;
}

EXTERN_C KS_RESULT STDCALL KsOpenFile(const char *pFileName, HKDECODE *ppHandle) {
    if (!pFileName || !pFileName[0]) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (!ppHandle) {
        return KS_ERR_INVALID_PARAMETER;
    }
    uint8 *buffer;
    KS_RESULT result;
#ifdef __CYGWIN__
    // Cygwin seems to have problem with `fopen` in C# P/Invoke calls, so I decide to use
    // Win32 API on Windows, instead of GNU C standard.
    // Problem description: one of these two results occurs (randomly), 1. infinite lock,
    // or 2. memory access violation.
    HANDLE hFile = CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile && hFile != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER fileSize;
        GetFileSizeEx(hFile, &fileSize);
        DWORD bufferSize = fileSize.LowPart;
        buffer = new uint8[bufferSize];
        DWORD bytesRead;
        ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL);
        CloseHandle(hFile);
        result = KsOpenBuffer(buffer, bufferSize, TRUE, ppHandle);
    } else {
        result = KS_ERR_FILE_OP_FAILED;
    }
#else
    FILE *fp = fopen(pFileName, "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        if (fileSize > UINT_MAX || fileSize <= 0) {
            fclose(fp);
            return KS_ERR_INVALID_FILE_PROP;
        }
        buffer = new uint8[fileSize];
        fseek(fp, 0, SEEK_SET);
        fread(buffer, (size_t)fileSize, 1, fp);
        fclose(fp);
        result = KsOpenBuffer(buffer, (uint32)fileSize, TRUE, ppHandle);
        delete[] buffer;
    } else {
        *ppHandle = NULL;
        return KS_ERR_FILE_OP_FAILED;
    }
#endif
    return result;
}

EXTERN_C KS_RESULT STDCALL KsOpenBuffer(uint8 *pData, uint32 dwDataSize, ubool bClone, HKDECODE *ppHandle) {
    if (!pData || dwDataSize <= 0 || !ppHandle) {
        return KS_ERR_INVALID_PARAMETER;
    }
    *ppHandle = NULL;
    auto decode = new KS_DECODE();
    memset(decode, 0, sizeof(KS_DECODE));
    decode->cb = sizeof(KS_DECODE);
    decode->magic = KS_DECODE_MAGIC;
    decode->closed = FALSE;
    if (bClone) {
        decode->dataIsCopy = TRUE;
        decode->data = new uint8[dwDataSize];
        memcpy(decode->data, pData, dwDataSize);
    } else {
        decode->dataIsCopy = FALSE;
        decode->data = pData;
    }
    decode->dataSize = dwDataSize;
    decode->status.stage = KS_STAGE_INITIALIZED;
    *ppHandle = decode;
    return KS_ERR_OK;
}

EXTERN_C KS_RESULT STDCALL KsSetParamI32(HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint32 dwParam) {
    if (!hDecode) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed || decode->status.stage != KS_STAGE_INITIALIZED) {
        return KS_ERR_INVALID_OPERATION;
    }
    switch (dwParamType) {
        case KS_PARAM_KEY1:
            decode->params.key1 = dwParam;
            break;
        case KS_PARAM_KEY2:
            decode->params.key2 = dwParam;
            break;
        case KS_PARAM_BUFFER_SIZE:
            decode->params.bufferSize = dwParam;
            break;
        default:
            return KS_ERR_INVALID_PARAMETER;
    }
    return KS_ERR_OK;
}

EXTERN_C KS_RESULT STDCALL KsSetParamI64(HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint64 qwParam) {
    if (!hDecode) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed || decode->status.stage != KS_STAGE_INITIALIZED) {
        return KS_ERR_INVALID_OPERATION;
    }
    switch (dwParamType) {
        case KS_PARAM_KEY:
            decode->params.key1 = (uint32)(qwParam & 0x00000000ffffffff);
            decode->params.key2 = (uint32)((qwParam & 0xffffffff00000000) >> 32);
            break;
        default:
            return KS_ERR_INVALID_PARAMETER;
    }
    return KS_ERR_OK;
}

EXTERN_C KS_RESULT STDCALL KsBeginDecode(HKDECODE hDecode) {
    if (!hDecode) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed || decode->status.stage != KS_STAGE_INITIALIZED) {
        return KS_ERR_INVALID_OPERATION;
    }
    CHcaDecoder *pHCA = new CHcaDecoder(decode->params.key1, decode->params.key2);
    decode->hca = pHCA;
    KS_RESULT r = pHCA->ReadHeader(decode->data, decode->dataSize, &decode->status);
    decode->status.hcaCheckFailed = KS_CALL_SUCCESSFUL(r) ? FALSE : TRUE;
    decode->status.hcaChecked = TRUE;
    if (KsIsHcaCheckPassed(hDecode)) {
        decode->status.stage = KS_STAGE_DECODE_STARTED;
    }
    return r;
}

EXTERN_C KS_RESULT STDCALL KsGetWaveHeader(HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize) {
    if (!hDecode) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (pBuffer && (!pdwDataSize || (*pdwDataSize <= 0))) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (!KsIsHcaCheckPassed(hDecode)) {
        return KS_ERR_INVALID_OPERATION;
    }
    if (decode->status.stage != KS_STAGE_DECODE_STARTED) {
        return KS_ERR_INVALID_OPERATION;
    }
    KS_RESULT result = decode->hca->GetWaveHeader(pBuffer, pdwDataSize);
    if (KS_CALL_SUCCESSFUL(result) && pBuffer) {
        decode->status.stage = KS_STAGE_HEADER_DECODED;
    }
    return result;
}

EXTERN_C KS_RESULT STDCALL KsDecodeData(HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize) {
    if (!hDecode) {
        return KS_ERR_INVALID_PARAMETER;
    }
    if (pBuffer && (!pdwDataSize || (*pdwDataSize <= 0))) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (!KsIsHcaCheckPassed(hDecode) || decode->closed) {
        return KS_ERR_INVALID_OPERATION;
    }
    if (decode->status.stage != KS_STAGE_HEADER_DECODED && decode->status.stage != KS_STAGE_DATA_DECODING) {
        return KS_ERR_INVALID_OPERATION;
    }
    ubool hasMore;
    KS_RESULT result = decode->hca->DecodeData(decode->data, decode->dataSize, &decode->status, pBuffer,
                                               pdwDataSize, &hasMore);
    if (KS_CALL_SUCCESSFUL(result) && pBuffer) {
        if (hasMore) {
            decode->status.stage = KS_STAGE_DATA_DECODING;
            return KS_OP_HAS_MORE_DATA;
        } else {
            decode->status.stage = KS_STAGE_DECODING_COMPLETE;
            return KS_ERR_OK;
        }
    }
    return result;
}

EXTERN_C KS_RESULT STDCALL KsEndDecode(HKDECODE hDecode) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage != KS_STAGE_INVALID) {
        decode->status.stage = KS_STAGE_DECODING_COMPLETE;
    }
    return KS_ERR_OK;
}

EXTERN_C KS_RESULT STDCALL KsCloseHandle(HKDECODE hDecode) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->hca) {
        delete decode->hca;
    }
    if (decode->dataIsCopy && decode->data) {
        delete[] decode->data;
    }
    memset(decode, 0, sizeof(KS_DECODE));
    decode->closed = TRUE;
    decode->status.stage = KS_STAGE_INVALID;
    delete decode;
    return KS_ERR_OK;
}

EXTERN_C KS_RESULT STDCALL KsGetHcaInfo(HKDECODE hDecode, HCA_INFO *pInfo) {
    if (!hDecode || !pInfo) {
        return KS_ERR_INVALID_PARAMETER;
    }
    memset(pInfo, 0, sizeof(HCA_INFO));
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (!KsIsHcaCheckPassed(hDecode)) {
        return KS_ERR_INVALID_OPERATION;
    }
    return decode->hca->GetInfo(pInfo);
}

EXTERN_C ubool STDCALL KsIsActiveHandle(HKDECODE hDecode) {
    if (!hDecode) {
        return FALSE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return FALSE;
    }
    if (decode->closed) {
        return FALSE;
    }
    if (!(decode->status.stage == KS_STAGE_INITIALIZED || decode->status.stage == KS_STAGE_DECODE_STARTED ||
          decode->status.stage == KS_STAGE_HEADER_DECODED || decode->status.stage == KS_STAGE_DATA_DECODING ||
          decode->status.stage == KS_STAGE_DECODING_COMPLETE)) {
        return FALSE;
    }
    return TRUE;
}

EXTERN_C ubool STDCALL KsIsHcaCheckPassed(HKDECODE hDecode) {
    if (!hDecode) {
        return FALSE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return FALSE;
    }
    if (decode->closed) {
        return FALSE;
    }
    return decode->status.hcaChecked ? (ubool)!decode->status.hcaCheckFailed : FALSE;
}

EXTERN_C KS_RESULT STDCALL KsHasMoreData(HKDECODE hDecode, ubool *pbHasMore) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    if (!pbHasMore) {
        return KS_ERR_INVALID_PARAMETER;
    }
    *pbHasMore = FALSE;
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage == KS_STAGE_DATA_DECODING || decode->status.stage == KS_STAGE_HEADER_DECODED) {
        *pbHasMore = TRUE;
        return KS_ERR_OK;
    } else if (decode->status.stage == KS_STAGE_DECODING_COMPLETE) {
        *pbHasMore = FALSE;
        return KS_ERR_OK;
    } else {
        *pbHasMore = FALSE;
        return KS_ERR_INVALID_OPERATION;
    }
}

EXTERN_C void STDCALL KsTest() {
}
