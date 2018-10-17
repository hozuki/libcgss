#include "CHcaCipherConfig.h"

CGSS_NS_BEGIN

    CHcaCipherConfig::CHcaCipherConfig()
        : MyBase() {
        memset(this, 0, sizeof(CHcaCipherConfig));
    }

    CHcaCipherConfig::CHcaCipherConfig(HcaCipherType cipherType)
        : MyClass() {
        keyParts.key1 = keyParts.key2 = 0;
        if (cipherType == HcaCipherType::WithKey) {
            cipherType = HcaCipherType::NoCipher;
        }
        this->cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(cipherType);
    }

    CHcaCipherConfig::CHcaCipherConfig(uint32_t key1, uint32_t key2)
        : MyClass() {
        Initialize(key1, key2, 0);
    }

    CHcaCipherConfig::CHcaCipherConfig(uint64_t key)
        : MyClass((uint32_t)(key >> 32), (uint32_t)(key & 0xffffffff)) {
    }

    CHcaCipherConfig::CHcaCipherConfig(uint32_t key1, uint32_t key2, uint16_t keyModifier)
        : MyClass() {
        Initialize(key1, key2, keyModifier);
    }

    CHcaCipherConfig::CHcaCipherConfig(uint64_t key, uint16_t keyModifier)
        : MyClass((uint32_t)(key >> 32), (uint32_t)(key & 0xffffffff), keyModifier) {
    }

    void CHcaCipherConfig::Initialize(uint32_t key1, uint32_t key2, uint16_t keyModifier) {
        if (key1 == 0 && key2 == 0) {
            cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(HcaCipherType::NoCipher);
        } else {
            cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(HcaCipherType::WithKey);
        }
        keyParts.key1 = key1;
        keyParts.key2 = key2;
        this->keyModifier = keyModifier;
    }

CGSS_NS_END
