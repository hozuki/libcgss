#pragma once

#include "../../cgss_env.h"
#include "../../cgss_enum.h"
#include "../../cdata/HCA_CIPHER_CONFIG.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CHcaCipherConfig final : public HCA_CIPHER_CONFIG {

    __extends(HCA_CIPHER_CONFIG, CHcaCipherConfig);

    public:

        CHcaCipherConfig();

        CHcaCipherConfig(const CHcaCipherConfig &) = default;

        explicit CHcaCipherConfig(HcaCipherType cipherType);

        CHcaCipherConfig(uint32_t key1, uint32_t key2);

        explicit CHcaCipherConfig(uint64_t key);

        CHcaCipherConfig(uint32_t key1, uint32_t key2, uint16_t keyModifier);

        CHcaCipherConfig(uint64_t key, uint16_t keyModifier);

    private:

        void Initialize(uint32_t key1, uint32_t key2, uint16_t keyModifier);

    };

CGSS_NS_END
