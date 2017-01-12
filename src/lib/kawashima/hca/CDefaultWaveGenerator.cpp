#include "CDefaultWaveGenerator.h"

CGSS_NS_BEGIN

    void CDefaultWaveGenerator::Decode8BitU(float data, uint8_t *buffer, uint32_t *cursor) {
        uint8_t u = (uint8_t)((int32_t)(data * 0x7f) + 0x80);
        *(uint8_t *)(buffer + *cursor) = u;
        *cursor += 1;
    }

    void CDefaultWaveGenerator::Decode16Bit(float data, uint8_t *buffer, uint32_t *cursor) {
        int16_t i = (int16_t)(data * 0x7fff);
        *(int16_t *)(buffer + *cursor) = i;
        *cursor += 2;
    }

    void CDefaultWaveGenerator::Decode24Bit(float data, uint8_t *buffer, uint32_t *cursor) {
        typedef struct _b24 {
            uint8_t a, b, c;
        } b24;
        typedef union _b32 {
            struct {
                b24 v;
                uint8_t a;
            };
            int32_t dummy;
        } b32;
        b32 v;
        int32_t i = (int32_t)(data * 0x7fffff);
        v.dummy = i;
        *(b24 *)(buffer + *cursor) = v.v;
        *cursor += 3;
    }

    void CDefaultWaveGenerator::Decode32Bit(float data, uint8_t *buffer, uint32_t *cursor) {
        int32_t i = (int32_t)(data * 0x7fffffff);
        *(int32_t *)(buffer + *cursor) = i;
        *cursor += 4;
    }

    void CDefaultWaveGenerator::DecodeFloat(float data, uint8_t *buffer, uint32_t *cursor) {
        *(float *)(buffer + *cursor) = data;
        *cursor += 4;
    }

CGSS_NS_END
