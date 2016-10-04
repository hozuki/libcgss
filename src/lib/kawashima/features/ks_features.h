#ifndef CGSS_KS_FEATURES_H
#define CGSS_KS_FEATURES_H

#include "../api/ks_decode.h"

typedef KS_RESULT (*KS_FEATURE_REG_HOOK)(KS_DECODE *hDecode);

ubool KsFeatureRegisterInitializer(KS_FEATURE_REG_HOOK hook);

ubool KsFeatureRegisterFinalizer(KS_FEATURE_REG_HOOK hook);

void KsFeatureCallInitializers(KS_DECODE *hDecode);

void KsFeatureCallFinalizers(KS_DECODE *hDecode);

KS_RESULT KsGetAvailableFeatures(KS_DECODE *hDecode, HCA_DECODE_FEATURE *pFeatures);

KS_RESULT KsFeatureStreamingInitializer(KS_DECODE *hDecode);

KS_RESULT KsFeatureStreamingFinalizer(KS_DECODE *hDecode);

#endif //CGSS_KS_FEATURES_H
