#include "CDefaultWaveGenerator.h"
#include "../../takamori/exceptions/CArgumentException.h"

CGSS_NS_BEGIN

    uint32_t CDefaultWaveGenerator::Decode8BitU(float data, uint8_t *buffer, const uint32_t cursor) {
        if (!buffer) {
            throw CArgumentException("CDefaultWaveGenerator::Decode8BitU");
        }
        uint8_t u = (uint8_t)((int32_t)(data * 0x7f) + 0x80);
        *(uint8_t *)(buffer + cursor) = u;
        return cursor + 1;
    }

    uint32_t CDefaultWaveGenerator::Decode16BitS(float data, uint8_t *buffer, const uint32_t cursor) {
        if (!buffer) {
            throw CArgumentException("CDefaultWaveGenerator::Decode16BitS");
        }
        int16_t i = (int16_t)(data * 0x7fff);
        *(int16_t *)(buffer + cursor) = i;
        return cursor + 2;
    }

    uint32_t CDefaultWaveGenerator::Decode24BitS(float data, uint8_t *buffer, const uint32_t cursor) {
        if (!buffer) {
            throw CArgumentException("CDefaultWaveGenerator::Decode24BitS");
        }
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
        *(b24 *)(buffer + cursor) = v.v;
        return cursor + 3;
    }

    uint32_t CDefaultWaveGenerator::Decode32BitS(float data, uint8_t *buffer, const uint32_t cursor) {
        if (!buffer) {
            throw CArgumentException("CDefaultWaveGenerator::Decode32BitS");
        }
        int32_t i = (int32_t)((double)data * 0x7fffffff);
        *(int32_t *)(buffer + cursor) = i;
        return cursor + 4;
    }

    uint32_t CDefaultWaveGenerator::DecodeFloat(float data, uint8_t *buffer, const uint32_t cursor) {
        if (!buffer) {
            throw CArgumentException("CDefaultWaveGenerator::DecodeFloat");
        }
        *(float *)(buffer + cursor) = data;
        return cursor + 4;
    }

CGSS_NS_END
