#pragma once

#include "../../../cgss_env.h"

CGSS_NS_BEGIN

    class CHcaData {

    public:

        CHcaData(uint8_t *data, uint32_t dataSize, uint32_t size);

        CHcaData(CHcaData &) = default;

        int32_t CheckBit(int32_t bitSize);

        int32_t GetBit(int32_t bitSize);

        void AddBit(int32_t bitSize);

    private:

        uint8_t *_data;
        uint32_t _dataSize;
        int32_t _size;
        int32_t _bit;

    };

CGSS_NS_END
