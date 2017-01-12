#pragma once

#include "../../../cgss_env.h"
#include "../../../cgss_enum.h"

CGSS_NS_BEGIN

    class CHcaCipher {

    public:

        CHcaCipher();

        CHcaCipher(HcaCipherType type, uint32_t key1, uint32_t key2);

        CHcaCipher(const CHcaCipher &);

        bool_t InitEncryptTable();

        void Decrypt(uint8_t *data, uint32_t size) const;

        void Encrypt(uint8_t *data, uint32_t size) const;

    private:

        bool_t Init(HcaCipherType type, uint32_t key1, uint32_t key2);

        static const uint32_t TableSize = 0x100;

        uint8_t _decryptTable[TableSize];
        uint8_t _encryptTable[TableSize];

        void Init0();

        void Init1();

        void Init56(uint32_t key1, uint32_t key2);

        void Init56_CreateTable(uint8_t *table, uint8_t key);

        HcaCipherType _cipherType;

    };

CGSS_NS_END
