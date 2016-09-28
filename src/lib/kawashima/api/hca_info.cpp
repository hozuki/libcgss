
#include <string.h>
#include "../hca_info.h"

_HCA_CIPHER_CONFIG::_HCA_CIPHER_CONFIG() {
    memset(this, 0, sizeof(HCA_CIPHER_CONFIG));
}

_HCA_CIPHER_CONFIG::_HCA_CIPHER_CONFIG(uint32 key1, uint32 key2) {
    if (key1 == 0 && key2 == 0) {
        cipherType = HCA_CIPHER_TYPE_NO_CIPHER;
    } else {
        cipherType = HCA_CIPHER_TYPE_WITH_KEY;
    }
    keyParts.key1 = key1;
    keyParts.key2 = key2;
}

_HCA_CIPHER_CONFIG::_HCA_CIPHER_CONFIG(uint64 key)
        : _HCA_CIPHER_CONFIG((uint32)(key >> 32), (uint32)(key & 0xffffffff)) {
}
