#include "CHcaData.h"

CGSS_NS_BEGIN

    CHcaData::CHcaData(uint8_t *data, uint32_t size) {
        _data = data;
        _size = size * 8 - 16;
        _bit = 0;
    }

    int32_t CHcaData::CheckBit(int32_t bitSize) {
        int32_t v = 0;
        if (_bit + bitSize <= _size) {
            static int32_t mask[] = {0xFFFFFF, 0x7FFFFF, 0x3FFFFF, 0x1FFFFF, 0x0FFFFF, 0x07FFFF, 0x03FFFF, 0x01FFFF};
            uint8_t *data = &_data[_bit >> 3];
            v = data[0];
            v = (v << 8) | data[1];
            v = (v << 8) | data[2];
            v &= mask[_bit & 7];
            v >>= 24 - (_bit & 7) - bitSize;
        }
        return v;
    }

    int32_t CHcaData::GetBit(int32_t bitSize) {
        int32_t v = CheckBit(bitSize);
        _bit += bitSize;
        return v;
    }

    void CHcaData::AddBit(int32_t bitSize) {
        _bit += bitSize;
    }

CGSS_NS_END
