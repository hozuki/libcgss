#pragma once

#include "../../../cgss_env.h"

CGSS_NS_BEGIN

    class CHcaData;

    class CHcaChannel {

    public:

        CHcaChannel();

        CHcaChannel(const CHcaChannel &) = delete;

        void Decode1(CHcaData *data, uint32_t a, int32_t b, const uint8_t *ath);

        void Decode2(CHcaData *data);

        void Decode3(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

        void Decode4(int32_t index, uint32_t a, uint32_t b, uint32_t c);

        void Decode5(int32_t index);

        float block[0x80];
        float base[0x80];
        int8_t value[0x80];
        int8_t scale[0x80];
        int8_t value2[8];
        int32_t type;
        int8_t *value3;
        uint32_t count;
        float wav1[0x80];
        float wav2[0x80];
        float wav3[0x80];
        float wave[8][0x80];

    };

CGSS_NS_END
