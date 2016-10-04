#ifndef CGSS_KS_EXTENSIONS_H
#define CGSS_KS_EXTENSIONS_H

#include "../api/ks_decode.h"

typedef KS_RESULT (*KS_EXTENSION_REG_HOOK)(KS_DECODE *hDecode);

ubool KsExtensionRegisterInitializer(KS_DECODE_EXTENSION extension, KS_EXTENSION_REG_HOOK hook);

ubool KsExtensionRegisterFinalizer(KS_DECODE_EXTENSION extension, KS_EXTENSION_REG_HOOK hook);

void KsExtensionCallInitializers(KS_DECODE *hDecode);

void KsExtensionCallFinalizers(KS_DECODE *hDecode);

KS_RESULT KsExtensionStreamingInitializer(KS_DECODE *hDecode);

KS_RESULT KsExtensionStreamingFinalizer(KS_DECODE *hDecode);

#endif //CGSS_KS_EXTENSIONS_H
