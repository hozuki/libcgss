#include "../../common/libcgss_building_dll.h"
#include "../../ks_api.h"
#include "ks_features.h"

EXTERN_C KS_RESULT STDCALL KsFeatStreamingGetSize(HKDECODE hDecode, uint32 *pdwSizeInBytes) {
    return KsGetWaveSize(hDecode, TRUE, pdwSizeInBytes);
}

EXTERN_C KS_RESULT STDCALL KsFeatStreamingRead(HKDECODE hDecode, uint8 *pBuffer, uint32 dwBufferSize, uint32 *pdwBytesRead) {

}

EXTERN_C KS_RESULT STDCALL KsFeatStreamingSeek(HKDECODE hDecode, uint32 dwPosition) {

}

EXTERN_C KS_RESULT STDCALL KsFeatStreamingTell(HKDECODE hDecode, uint32 *pdwPosition) {

}
