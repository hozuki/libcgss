#pragma once

#include "../../cgss_env.h"
#include "../../cgss_enum.h"
#include "../../data/HCA_CIPHER_CONFIG.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CHcaCipherConfig final : public HCA_CIPHER_CONFIG {

    public:

        CHcaCipherConfig();

        CHcaCipherConfig(const CHcaCipherConfig &) = default;

        CHcaCipherConfig(HcaCipherType cipherType);

        CHcaCipherConfig(uint32_t key1, uint32_t key2);

        CHcaCipherConfig(uint64_t key);

    };

CGSS_NS_END
