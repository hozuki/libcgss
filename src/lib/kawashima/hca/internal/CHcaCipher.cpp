#include "CHcaCipher.h"
#include "../../../cgss_cdata.h"

static void TransformKey(uint32_t key1, uint32_t key2, uint16_t mod, uint32_t *pk1, uint32_t *pk2) {
    auto key = (uint64_t)key2 << 32 | key1;
    auto k2 = ((uint64_t)mod << 16 | (uint16_t)(~mod + 2));

    auto newKey = key * k2;

    if (pk2) {
        *pk2 = (uint32_t)(newKey >> 32);
    }

    if (pk1) {
        *pk1 = (uint32_t)(newKey & 0xffffffff);
    }
}

CGSS_NS_BEGIN

    CHcaCipher::CHcaCipher() {
        Init(CHcaCipherConfig(HcaCipherType::NoCipher));
    }

    CHcaCipher::CHcaCipher(const HCA_CIPHER_CONFIG &config) {
        CHcaCipherConfig cfg(config.key, config.keyModifier);
        Init(cfg);
    }

    CHcaCipher::CHcaCipher(const CHcaCipherConfig &config) {
        Init(config);
    }

    CHcaCipher::CHcaCipher(const CHcaCipher &other) {
        _cipherType = other._cipherType;
        memcpy(_decryptTable, other._decryptTable, TableSize);
        memcpy(_encryptTable, other._encryptTable, TableSize);
    }

    bool_t CHcaCipher::Init(const CHcaCipherConfig &config) {
        auto type = static_cast<HcaCipherType>(config.cipherType);

        if (!(config.keyParts.key1 | config.keyParts.key2) && type == HcaCipherType::WithKey) {
            type = HcaCipherType::NoCipher;
        }

        uint32_t key1 = config.keyParts.key1, key2 = config.keyParts.key2;

        if ((key1 && key2) && type == HcaCipherType::WithKey && config.keyModifier) {
            TransformKey(key1, key2, config.keyModifier, &key1, &key2);
        }

        switch (type) {
            case HcaCipherType::NoCipher:
                Init0();
                break;
            case HcaCipherType::Static:
                Init1();
                break;
            case HcaCipherType::WithKey:
                Init56(key1, key2);
                break;
        }

        _cipherType = type;

        return InitEncryptTable();
    }

    void CHcaCipher::Decrypt(uint8_t *data, uint32_t size) const {
        for (uint8_t *d = data; size > 0; d++, size--) {
            *d = _decryptTable[*d];
        }
    }

    void CHcaCipher::Encrypt(uint8_t *data, uint32_t size) const {
        for (uint8_t *d = data; size > 0; d++, size--) {
            *d = _encryptTable[*d];
        }
    }

    void CHcaCipher::Init0() {
        for (uint32_t i = 0; i < TableSize; i++) {
            _decryptTable[i] = (uint8_t)i;
        }
    }

    void CHcaCipher::Init1() {
        for (uint32_t i = 1, v = 0; i < 0xFF; i++) {
            v = (v * 13 + 11) & 0xFF;
            if (v == 0 || v == 0xFF) {
                v = (v * 13 + 11) & 0xFF;
            }
            _decryptTable[i] = (uint8_t)v;
        }
        _decryptTable[0] = 0;
        _decryptTable[0xFF] = 0xFF;
    }

    void CHcaCipher::Init56(uint32_t key1, uint32_t key2) {
        // Generate table #1
        uint8_t t1[8];
        if (!key1) {
            key2--;
        }
        key1--;
        for (uint8_t i = 0; i < 7; i++) {
            t1[i] = (uint8_t)key1;
            key1 = (key1 >> 8) | (key2 << 24);
            key2 >>= 8;
        }

        // Generate table #2
        uint8_t t2[0x10] = {
            t1[1], (uint8_t)(t1[1] ^ t1[6]),
            (uint8_t)(t1[2] ^ t1[3]), t1[2],
            (uint8_t)(t1[2] ^ t1[1]), (uint8_t)(t1[3] ^ t1[4]),
            t1[3], (uint8_t)(t1[3] ^ t1[2]),
            (uint8_t)(t1[4] ^ t1[5]), t1[4],
            (uint8_t)(t1[4] ^ t1[3]), (uint8_t)(t1[5] ^ t1[6]),
            t1[5], (uint8_t)(t1[5] ^ t1[4]),
            (uint8_t)(t1[6] ^ t1[1]), t1[6],
        };

        // Generate table #3
        uint8_t t3[0x100], t31[0x10], t32[0x10], *t = t3;
        Init56_CreateTable(t31, t1[0]);
        for (uint8_t i = 0; i < 0x10; i++) {
            Init56_CreateTable(t32, t2[i]);
            uint8_t v = t31[i] << 4;
            for (uint8_t j = 0; j < 0x10; j++) {
                *(t++) = v | t32[j];
            }
        }

        // Generate CIPH table
        t = &_decryptTable[1];
        for (uint32_t i = 0, v = 0; i < TableSize; i++) {
            v = (v + 0x11) & 0xFF;
            uint8_t a = t3[v];
            if (a != 0 && a != 0xFF) {
                *(t++) = a;
            }
        }
        _decryptTable[0] = 0;
        _decryptTable[0xFF] = 0xFF;
    }

    void CHcaCipher::Init56_CreateTable(uint8_t *r, uint8_t key) {
        int32_t mul = ((key & 1) << 3) | 5;
        int32_t add = (key & 0xE) | 1;
        key >>= 4;
        for (uint32_t i = 0; i < 0x10; i++) {
            key = (uint8_t)((key * mul + add) & 0xF);
            *(r++) = key;
        }
    }

    bool_t CHcaCipher::InitEncryptTable() {
        memset(_encryptTable, 0, TableSize);
        for (uint32_t i = 0; i < TableSize; ++i) {
            _encryptTable[_decryptTable[i]] = (uint8_t)i;
        }
        return TRUE;
    }

CGSS_NS_END
