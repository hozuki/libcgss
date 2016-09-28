/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include "CHcaData.h"

CHcaData::CHcaData(uint8 *data, uint32 size) {
    _data = data;
    _size = size * 8 - 16;
    _bit = 0;
}

int32 CHcaData::CheckBit(int32 bitSize) {
    int32 v = 0;
    if (_bit + bitSize <= _size) {
        static int32 mask[] = {0xFFFFFF, 0x7FFFFF, 0x3FFFFF, 0x1FFFFF, 0x0FFFFF, 0x07FFFF, 0x03FFFF, 0x01FFFF};
        uint8 *data = &_data[_bit >> 3];
        v = data[0];
        v = (v << 8) | data[1];
        v = (v << 8) | data[2];
        v &= mask[_bit & 7];
        v >>= 24 - (_bit & 7) - bitSize;
    }
    return v;
}

int32 CHcaData::GetBit(int32 bitSize) {
    int32 v = CheckBit(bitSize);
    _bit += bitSize;
    return v;
}

void CHcaData::AddBit(int32 bitSize) {
    _bit += bitSize;
}
