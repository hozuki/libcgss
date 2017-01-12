#include "../../cgss_env.h"

CGSS_NS_BEGIN

    struct WaveRiffSection {
        uint8_t riff[4];
        uint32_t riffSize;
        uint8_t wave[4];
        uint8_t fmt[4];
        uint32_t fmtSize;
        uint16_t fmtType;
        uint16_t fmtChannelCount;
        uint32_t fmtSamplingRate;
        uint32_t fmtSamplesPerSec;
        uint16_t fmtSamplingSize;
        uint16_t fmtBitCount;
    };

    struct WaveSampleSection {
        uint8_t smpl[4];
        uint32_t smplSize;
        uint32_t manufacturer;
        uint32_t product;
        uint32_t samplePeriod;
        uint32_t midiUnityNote;
        uint32_t midiPitchFraction;
        uint32_t smpteFormat;
        uint32_t smpteOffset;
        uint32_t sampleLoops;
        uint32_t samplerData;
        uint32_t loopIdentifier;
        uint32_t loopType;
        uint32_t loopStart;
        uint32_t loopEnd;
        uint32_t loopFraction;
        uint32_t loopPlayCount;
    };

    struct WaveNoteSection {
        uint8_t note[4];
        uint32_t noteSize;
        uint32_t dwName;
    };

    struct WaveDataSection {
        uint8_t data[4];
        uint32_t dataSize;
    };

CGSS_NS_END
