#include "WaveGen.h"

WaveGen::WaveGen() {
}

void WaveGen::Decode8BitS(float data, uint8 *buffer, uint32 *cursor) {
    uint8 u = (uint8)((int32)(data * 0x7f) + 0x80);
    *(uint8 *)(buffer + *cursor) = u;
    *cursor += 1;
}

void WaveGen::Decode16Bit(float data, uint8 *buffer, uint32 *cursor) {
    int16 i = (int16)(data * 0x7fff);
    *(int16 *)(buffer + *cursor) = i;
    *cursor += 2;
}

void WaveGen::Decode24Bit(float data, uint8 *buffer, uint32 *cursor) {
    typedef struct _b24 {
        uint8 a, b, c;
    } b24;
    typedef union _b32 {
        struct {
            b24 v;
            uint8 a;
        };
        int32 dummy;
    } b32;
    b32 v;
    int32 i = (int32)(data * 0x7fffff);
    v.dummy = i;
    *(b24 *)(buffer + *cursor) = v.v;
    *cursor += 3;
}

void WaveGen::Decode32Bit(float data, uint8 *buffer, uint32 *cursor) {
    int32 i = (int32)(data * 0x7fffffff);
    *(int32 *)(buffer + *cursor) = i;
    *cursor += 4;
}

void WaveGen::DecodeFloat(float data, uint8 *buffer, uint32 *cursor) {
    *(float *)(buffer + *cursor) = data;
    *cursor += 4;
}
