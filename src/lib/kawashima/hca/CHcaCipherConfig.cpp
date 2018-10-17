#include "CHcaCipherConfig.h"

static void TransformKey(uint32_t key1, uint32_t key2, uint16_t mod, uint32_t *pk1, uint32_t *pk2) {
    auto key = (uint64_t)key1 << 32 | key2;
    auto k2 = ((uint64_t)mod << 16 | (uint16_t)(~mod + 2));

    auto newKey = key * k2;

    if (pk1) {
        *pk1 = (uint32_t)(newKey >> 32);
    }

    if (pk2) {
        *pk2 = (uint32_t)(newKey & 0xffffffff);
    }
}

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
        Initialize(key1, key2);
    }

    CHcaCipherConfig::CHcaCipherConfig(uint64_t key)
        : MyClass((uint32_t)(key >> 32), (uint32_t)(key & 0xffffffff)) {
    }

    CHcaCipherConfig::CHcaCipherConfig(uint32_t key1, uint32_t key2, uint16_t keyModifier)
        : MyClass() {
        if ((key1 == 0 && key2 == 0) || keyModifier == 0) {
            Initialize(key1, key2);
        } else {
            uint32_t newKey1, newKey2;

            TransformKey(key1, key2, keyModifier, &newKey1, &newKey2);

            Initialize(newKey1, newKey2);
        }
    }

    CHcaCipherConfig::CHcaCipherConfig(uint64_t key, uint16_t keyModifier)
        : MyClass((uint32_t)(key >> 32), (uint32_t)(key & 0xffffffff), keyModifier) {
    }

    void CHcaCipherConfig::Initialize(uint32_t key1, uint32_t key2) {
        if (key1 == 0 && key2 == 0) {
            cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(HcaCipherType::NoCipher);
        } else {
            cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(HcaCipherType::WithKey);
        }
        keyParts.key1 = key1;
        keyParts.key2 = key2;
    }

CGSS_NS_END
