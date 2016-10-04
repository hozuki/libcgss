#ifndef CGSS_KS_DECODE_MAGIC_H
#define CGSS_KS_DECODE_MAGIC_H

#include "../../ks_api.h"
#include "ks_decode.h"

extern const uint32 KS_DECODE_MAGIC;

inline ubool KsCheckMagic(KS_DECODE *hDecode) {
    return hDecode->magic == KS_DECODE_MAGIC ? TRUE : FALSE;
}

inline ubool KsIsExtensionEnabled(KS_DECODE *hDecode, KS_DECODE_EXTENSION extension) {
    return (ubool)(hDecode->enabledExtensions & extension);
}

inline ubool KsIsExtensionAvailable(KS_DECODE *hDecode, KS_DECODE_EXTENSION extension) {
    return (ubool)(hDecode->availableExtensions & extension);
}

inline uint32 KsGetWaveHeaderDataSize(KS_DECODE *hDecode) {
    uint32 size;
    KsGetWaveHeader(hDecode, nullptr, &size);
    return size;
}

inline uint32 KsGetWaveBlockDataSize(KS_DECODE *hDecode) {
    uint32 size;
    KsDecodeData(hDecode, nullptr, &size);
    return size;
}

#endif //CGSS_KS_DECODE_MAGIC_H
