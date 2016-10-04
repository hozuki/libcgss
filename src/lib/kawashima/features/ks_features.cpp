#include <list>
#include "../../common/libcgss_building_dll.h"
#include "../../ks_api.h"
#include "../api/ks_decode_magic.h"
#include "ks_features.h"

static std::list<KS_FEATURE_REG_HOOK> __ks_init_hooks;
static std::list<KS_FEATURE_REG_HOOK> __ks_fin_hooks;

inline ubool check_magic(KS_DECODE *hDecode) {
    return hDecode->magic == KS_DECODE_MAGIC ? TRUE : FALSE;
}

CGSS_API_TYPE(KS_RESULT) KsEnableFeature(HKDECODE hDecode, HCA_DECODE_FEATURE dwFeature, ubool bEnabled) {
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
    if (decode->status.stage != KS_STAGE_INITIALIZED) {
        return KS_ERR_INVALID_OPERATION;
    }
    // If the 'feature' contains values outside current enum range, then the argument is corrupted.
    if (dwFeature & ~decode->availableFeatures) {
        return KS_ERR_FEATURE_UNAVAILABLE;
    }
    if (bEnabled) {
        decode->enabledFeatures = (HCA_DECODE_FEATURE)(decode->enabledFeatures | dwFeature);
    } else {
        decode->enabledFeatures = (HCA_DECODE_FEATURE)(decode->enabledFeatures & ~dwFeature);
    }
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsIsFeatureEnabled(HKDECODE hDecode, HCA_DECODE_FEATURE dwFeature, ubool *pbIsEnabled) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!check_magic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    *pbIsEnabled = (decode->enabledFeatures & dwFeature);
    return KS_ERR_OK;
}

ubool KsFeatureRegisterInitializer(KS_FEATURE_REG_HOOK hook) {
    if (hook) {
        // TODO: existence check.
        __ks_init_hooks.push_back(hook);
        return TRUE;
    } else {
        return FALSE;
    }
}

ubool KsFeatureRegisterFinalizer(KS_FEATURE_REG_HOOK hook) {
    if (hook) {
        // TODO: existence check.
        __ks_fin_hooks.push_front(hook);
        return TRUE;
    } else {
        return FALSE;
    }
}

void KsFeatureCallInitializers(KS_DECODE *hDecode) {
    for (auto hook : __ks_init_hooks) {
        if (hook) {
            hook(hDecode);
        }
    }
}

void KsFeatureCallFinalizers(KS_DECODE *hDecode) {
    for (auto hook:__ks_fin_hooks) {
        if (hook) {
            hook(hDecode);
        }
    }
}

KS_RESULT KsGetAvailableFeatures(KS_DECODE *hDecode, HCA_DECODE_FEATURE *pFeatures) {
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
    auto features = HCA_DECODE_FEATURE_NONE;
    HCA_INFO hcaInfo;
    auto result = hDecode->hca->GetInfo(hcaInfo);
    if (!hcaInfo.loopExists) {
        features = (HCA_DECODE_FEATURE)(features | HCA_DECODE_FEATURE_STREAMING);
    }
    *pFeatures = features;
    return KS_ERR_OK;
}

KS_RESULT KsFeatureStreamingInitializer(KS_DECODE *hDecode) {
    ubool isEnabled;
    auto r = KsIsFeatureEnabled(hDecode, HCA_DECODE_FEATURE_STREAMING, &isEnabled);
    if (!KS_CALL_SUCCESSFUL(r) || !isEnabled) {
        return KS_ERR_FEATURE_UNAVAILABLE;
    }
    uint32 waveTotalSize;
    r = KsGetWaveSize(hDecode, TRUE, &waveTotalSize);
    if (!KS_CALL_SUCCESSFUL(r)) {
        return KS_ERR_INVALID_INTERNAL_STATE;
    }
    auto streamingStatus = new HCA_FEAT_STREAMING_STATUS();
    streamingStatus->streamingDataSize = waveTotalSize;
    streamingStatus->streamingData = new uint8[waveTotalSize];
    hDecode->streamingStatus = streamingStatus;
    return KS_ERR_OK;
}

KS_RESULT KsFeatureStreamingFinalizer(KS_DECODE *hDecode) {
    if (!(hDecode->availableFeatures & HCA_DECODE_FEATURE_STREAMING)) {
        return KS_ERR_FEATURE_UNAVAILABLE;
    }
    if (!(hDecode->enabledFeatures & HCA_DECODE_FEATURE_STREAMING)) {
        return KS_ERR_FEATURE_NOT_ENABLED;
    }
    if (!hDecode->streamingStatus) {
        return KS_ERR_INVALID_PARAMETER;
    }
    delete[] hDecode->streamingStatus->streamingData;
    hDecode->streamingStatus->streamingData = nullptr;
    delete hDecode->streamingStatus;
    hDecode->streamingStatus = nullptr;
}
