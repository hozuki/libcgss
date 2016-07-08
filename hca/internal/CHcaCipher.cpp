#include <string.h>
#include "CHcaCipher.h"

CHcaCipher::CHcaCipher() {
    Init(HCA_CIPHER_TYPE_NO_CIPHER, 0, 0);
}

ubool CHcaCipher::Init(HCA_CIPHER_TYPE type, uint32 key1, uint32 key2) {
    if (!(key1 | key2) && type == HCA_CIPHER_TYPE_WITH_KEY) {
        type = HCA_CIPHER_TYPE_NO_CIPHER;
    }
    switch (type) {
        case HCA_CIPHER_TYPE_NO_CIPHER:
            Init0();
            break;
        case HCA_CIPHER_TYPE_STATIC:
            Init1();
            break;
        case HCA_CIPHER_TYPE_WITH_KEY:
            Init56(key1, key2);
            break;
        default:
            return FALSE;
    }
    _cipherType = type;
    return InitEncryptTable();
}

void CHcaCipher::Decrypt(uint8 *data, uint32 size) {
    for (uint8 *d = data; size > 0; d++, size--) {
        *d = _decryptTable[*d];
    }
}

void CHcaCipher::Encrypt(uint8 *data, uint32 size) {
    for (uint8 *d = data; size > 0; d++, size--) {
        *d = _encryptTable[*d];
    }
}

void CHcaCipher::Init0() {
    for (uint32 i = 0; i < 0x100; i++) {
        _decryptTable[i] = (uint8)i;
    }
}

void CHcaCipher::Init1() {
    for (uint32 i = 1, v = 0; i < 0xFF; i++) {
        v = (v * 13 + 11) & 0xFF;
        if (v == 0 || v == 0xFF) {
            v = (v * 13 + 11) & 0xFF;
        }
        _decryptTable[i] = (uint8)v;
    }
    _decryptTable[0] = 0;
    _decryptTable[0xFF] = 0xFF;
}

void CHcaCipher::Init56(uint32 key1, uint32 key2) {
    // Generate table #1
    uint8 t1[8];
    if (!key1) {
        key2--;
    }
    key1--;
    for (uint8 i = 0; i < 7; i++) {
        t1[i] = (uint8)key1;
        key1 = (key1 >> 8) | (key2 << 24);
        key2 >>= 8;
    }

    // Generate table #2
    uint8 t2[0x10] = {
            t1[1], (uint8)(t1[1] ^ t1[6]),
            (uint8)(t1[2] ^ t1[3]), t1[2],
            (uint8)(t1[2] ^ t1[1]), (uint8)(t1[3] ^ t1[4]),
            t1[3], (uint8)(t1[3] ^ t1[2]),
            (uint8)(t1[4] ^ t1[5]), t1[4],
            (uint8)(t1[4] ^ t1[3]), (uint8)(t1[5] ^ t1[6]),
            t1[5], (uint8)(t1[5] ^ t1[4]),
            (uint8)(t1[6] ^ t1[1]), t1[6],
    };

    // Generate table #3
    uint8 t3[0x100], t31[0x10], t32[0x10], *t = t3;
    Init56_CreateTable(t31, t1[0]);
    for (uint8 i = 0; i < 0x10; i++) {
        Init56_CreateTable(t32, t2[i]);
        uint8 v = t31[i] << 4;
        for (uint8 j = 0; j < 0x10; j++) {
            *(t++) = v | t32[j];
        }
    }

    // Generate CIPH table
    t = &_decryptTable[1];
    for (uint32 i = 0, v = 0; i < 0x100; i++) {
        v = (v + 0x11) & 0xFF;
        uint8 a = t3[v];
        if (a != 0 && a != 0xFF) {
            *(t++) = a;
        }
    }
    _decryptTable[0] = 0;
    _decryptTable[0xFF] = 0xFF;
}

void CHcaCipher::Init56_CreateTable(uint8 *r, uint8 key) {
    int32 mul = ((key & 1) << 3) | 5;
    int32 add = (key & 0xE) | 1;
    key >>= 4;
    for (uint32 i = 0; i < 0x10; i++) {
        key = (uint8)((key * mul + add) & 0xF);
        *(r++) = key;
    }
}

ubool CHcaCipher::InitEncryptTable() {
    ubool b;
    memset(_encryptTable, 0, 0x100);
    for (uint32 i = 0; i < 0x100; ++i) {
        b = FALSE;
        for (uint32 j = 0; j < 0x100; ++j) {
            if (_decryptTable[j] == i) {
                _encryptTable[i] = (uint8)(j & 0xff);
                b = TRUE;
                break;
            }
        }
        if (!b) {
            return FALSE;
        }
    }
    return TRUE;
}
