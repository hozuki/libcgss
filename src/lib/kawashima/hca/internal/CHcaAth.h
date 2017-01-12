#pragma once

#include "../../../cgss_env.h"

CGSS_NS_BEGIN

    class CHcaAth {

    public:

        CHcaAth();

        CHcaAth(const CHcaAth &) = default;

        bool_t Init(uint16_t type, uint32_t key);

        const uint8_t *GetTable();

    private:

        uint8_t _table[0x80];

        void Init0();

        void Init1(uint32_t key);

    };


CGSS_NS_END
