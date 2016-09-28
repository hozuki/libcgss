/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef KAWASHIMA_WAVEGEN_H
#define KAWASHIMA_WAVEGEN_H

#include "../kstypedef.h"

struct WaveGen final {

public:
    static void Decode8BitS(float data, uint8 *buffer, uint32 *cursor);

    static void Decode16Bit(float data, uint8 *buffer, uint32 *cursor);

    static void Decode24Bit(float data, uint8 *buffer, uint32 *cursor);

    static void Decode32Bit(float data, uint8 *buffer, uint32 *cursor);

    static void DecodeFloat(float data, uint8 *buffer, uint32 *cursor);

private:
    WaveGen();

};

#endif //KAWASHIMA_WAVEGEN_H
