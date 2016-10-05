#include <map>
#include "../../common/cgss_building_dll.h"
#include "../../ks_api.h"
#include "../api/ks_decode_helper.h"
#include "ks_ext.h"

static std::map<KS_DECODE_EXTENSION, KS_EXTENSION_REG_HOOK> __ks_init_hooks;
static std::map<KS_DECODE_EXTENSION, KS_EXTENSION_REG_HOOK> __ks_fin_hooks;

CGSS_API_TYPE(KS_RESULT) KsPrepareExtensions(KS_DECODE_HANDLE hDecode) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!KsCheckMagic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage != KS_STAGE_DECODE_STARTED) {
        return KS_ERR_INVALID_OPERATION;
    }
    if (decode->extensionsPrepared) {
        return KS_ERR_OK;
    }
    KsExtensionCallInitializers(decode);
    decode->extensionsPrepared = TRUE;
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsEnableExtension(KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION dwExtension, ubool bEnabled) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!KsCheckMagic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage != KS_STAGE_DECODE_STARTED) {
        return KS_ERR_INVALID_OPERATION;
    }
    // If the extension contains values outside current enum range, then the argument is corrupted.
    ubool isExtensionAvailable;
    auto r = KsIsExtensionAvailable(hDecode, dwExtension, &isExtensionAvailable);
    if (!KS_CALL_SUCCESSFUL(r)) {
        return r;
    }
    if (!isExtensionAvailable) {
        return KS_ERR_EXTENSION_NOT_AVAILABLE;
    }
    if (bEnabled) {
        decode->enabledExtensions = (KS_DECODE_EXTENSION)(decode->enabledExtensions | dwExtension);
    } else {
        decode->enabledExtensions = (KS_DECODE_EXTENSION)(decode->enabledExtensions & ~dwExtension);
    }
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsIsExtensionEnabled(KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION dwExtension, ubool *pbIsEnabled) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    if (!pbIsEnabled) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!KsCheckMagic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage < KS_STAGE_DECODE_STARTED) {
        return KS_ERR_INVALID_OPERATION;
    }
    *pbIsEnabled = (decode->enabledExtensions & dwExtension);
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsIsExtensionAvailable(KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION dwExtension, ubool *pbIsAvailable) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    if (!pbIsAvailable) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!KsCheckMagic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage < KS_STAGE_DECODE_STARTED) {
        return KS_ERR_INVALID_OPERATION;
    }
    *pbIsAvailable = (ubool)(decode->availableExtensions & dwExtension);
    return KS_ERR_OK;
}

CGSS_API_TYPE(KS_RESULT) KsGetAvailableExtensions(KS_DECODE_HANDLE hDecode, KS_DECODE_EXTENSION *pdwExtensions) {
    if (!hDecode) {
        return KS_ERR_INVALID_HANDLE;
    }
    if (!pdwExtensions) {
        return KS_ERR_INVALID_PARAMETER;
    }
    KS_DECODE *decode = (KS_DECODE *)hDecode;
    if (!KsCheckMagic(decode)) {
        return KS_ERR_MAGIC_NOT_MATCH;
    }
    if (decode->closed) {
        return KS_ERR_ALREADY_CLOSED;
    }
    if (decode->status.stage < KS_STAGE_DECODE_STARTED) {
        return KS_ERR_INVALID_OPERATION;
    }
    auto extensions = KS_EXTENSION_NONE;
    HCA_INFO hcaInfo;
    decode->hca->GetInfo(hcaInfo);
    if (!hcaInfo.loopExists) {
        extensions = (KS_DECODE_EXTENSION)(extensions | KS_EXTENSION_STREAMING);
    }
    *pdwExtensions = extensions;
    return KS_ERR_OK;
}

ubool KsExtensionRegisterInitializer(KS_DECODE_EXTENSION extension, KS_EXTENSION_REG_HOOK hook) {
    if (hook) {
        // TODO: existence check.
        __ks_init_hooks[extension] = hook;
        return TRUE;
    } else {
        return FALSE;
    }
}

ubool KsExtensionRegisterFinalizer(KS_DECODE_EXTENSION extension, KS_EXTENSION_REG_HOOK hook) {
    if (hook) {
        // TODO: existence check.
        __ks_fin_hooks[extension] = hook;
        return TRUE;
    } else {
        return FALSE;
    }
}

void KsExtensionCallInitializers(KS_DECODE *hDecode) {
    for (auto kv : __ks_init_hooks) {
        if (kv.second) {
            kv.second(hDecode);
        }
    }
}

void KsExtensionCallFinalizers(KS_DECODE *hDecode) {
    if (!hDecode->extensionsPrepared) {
        return;
    }
    for (auto kv:__ks_fin_hooks) {
        if (kv.second) {
            kv.second(hDecode);
        }
    }
}
