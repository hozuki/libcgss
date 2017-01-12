#include "CHcaCipherConfig.h"

CGSS_NS_BEGIN

    CHcaCipherConfig::CHcaCipherConfig() {
        memset(this, 0, sizeof(CHcaCipherConfig));
    }

    CHcaCipherConfig::CHcaCipherConfig(HcaCipherType cipherType) {
        keyParts.key1 = keyParts.key2 = 0;
        if (cipherType == HcaCipherType::WithKey) {
            cipherType = HcaCipherType::NoCipher;
        }
    }

    CHcaCipherConfig::CHcaCipherConfig(uint32_t key1, uint32_t key2) {
        if (key1 == 0 && key2 == 0) {
            cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(HcaCipherType::NoCipher);
        } else {
            cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(HcaCipherType::WithKey);
        }
        keyParts.key1 = key1;
        keyParts.key2 = key2;
    }

    CHcaCipherConfig::CHcaCipherConfig(uint64_t key)
            : CHcaCipherConfig((uint32_t)(key >> 32), (uint32_t)(key & 0xffffffff)) {
    }

CGSS_NS_END
