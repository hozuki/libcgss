#pragma once

#include "../../cgss_env.h"
#include "../../cdata/HCA_DECODER_CONFIG.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CHcaDecoderConfig final : public HCA_DECODER_CONFIG {

        __extends(HCA_DECODER_CONFIG, CHcaDecoderConfig);

    public:

        CHcaDecoderConfig();

        CHcaDecoderConfig(const CHcaDecoderConfig &) = default;

    };

CGSS_NS_END
