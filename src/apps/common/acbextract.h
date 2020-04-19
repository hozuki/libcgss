#pragma once

#include <string>

#include "../cgssh.h"
#include "../../lib/cgss_api.h"

struct AcbWalkOptions;

struct AcbWalkCallbackParams {

    AcbWalkCallbackParams();

    AcbWalkOptions *walkOptions;
    cgss::CAcbFile *acb;
    uint32_t formatVersion;
    std::string extractPathHint;
    const cgss::CAfs2Archive *archive;
    cgss::IStream *archiveDataStream;
    cgss::IStream *entryDataStream;
    bool_t isInternal;

    struct _cueinfo {

        inline _cueinfo()
            : id(0), offset(0), size(0) {
        }

        uint32_t id;
        uint64_t offset;
        uint64_t size;

    } cueInfo;

};

typedef int (*AcbWalkCallback)(AcbWalkCallbackParams *params);

struct AcbWalkOptions {

    AcbWalkOptions();

    HCA_DECODER_CONFIG decoderConfig;
    bool_t useCueName;
    bool_t byTrackIndex;
    AcbWalkCallback callback;

};

int AcbWalk(const std::string &inputAcbFile, AcbWalkOptions *options);
