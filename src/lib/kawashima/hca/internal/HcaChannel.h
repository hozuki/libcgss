/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef KAWASHIMA_HCACHANNEL_H
#define KAWASHIMA_HCACHANNEL_H

#include "../../kstypedef.h"
#include "CHcaData.h"

struct HcaChannel {
    float block[0x80];
    float base[0x80];
    int8 value[0x80];
    int8 scale[0x80];
    int8 value2[8];
    int32 type;
    int8 *value3;
    uint32 count;
    float wav1[0x80];
    float wav2[0x80];
    float wav3[0x80];
    float wave[8][0x80];

    void Decode1(CHcaData *data, uint32 a, int32 b, const uint8 *ath);

    void Decode2(CHcaData *data);

    void Decode3(uint32 a, uint32 b, uint32 c, uint32 d);

    void Decode4(int32 index, uint32 a, uint32 b, uint32 c);

    void Decode5(int32 index);
};

#endif //KAWASHIMA_HCACHANNEL_H
