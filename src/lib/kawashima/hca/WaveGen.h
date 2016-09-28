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
