/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef KAWASHIMA_WAVENATIVE_H
#define KAWASHIMA_WAVENATIVE_H

#include "../kstypedef.h"

struct WaveRiffSection {
    uint8 riff[4];
    uint32 riffSize;
    uint8 wave[4];
    uint8 fmt[4];
    uint32 fmtSize;
    uint16 fmtType;
    uint16 fmtChannelCount;
    uint32 fmtSamplingRate;
    uint32 fmtSamplesPerSec;
    uint16 fmtSamplingSize;
    uint16 fmtBitCount;
};

struct WaveSampleSection {
    uint8 smpl[4];
    uint32 smplSize;
    uint32 manufacturer;
    uint32 product;
    uint32 samplePeriod;
    uint32 MIDIUnityNote;
    uint32 MIDIPitchFraction;
    uint32 SMPTEFormat;
    uint32 SMPTEOffset;
    uint32 sampleLoops;
    uint32 samplerData;
    uint32 loopIdentifier;
    uint32 loopType;
    uint32 loopStart;
    uint32 loopEnd;
    uint32 loopFraction;
    uint32 loopPlayCount;
};

struct WaveNoteSection {
    uint8 note[4];
    uint32 noteSize;
    uint32 dwName;
};

struct WaveDataSection {
    uint8 data[4];
    uint32 dataSize;
};

#endif //KAWASHIMA_WAVESTRUCTURE_H
