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

typedef enum _ACB_EXTRACT_DISCOVERY {
    ACB_EXTRACT_DISCOVER_DEFAULT = 0,
    ACB_EXTRACT_DISCOVER_BY_TRACK = 1,
    ACB_EXTRACT_DISCOVER_BY_CUE = 2,
} ACB_EXTRACT_DISCOVERY;

struct AcbWalkOptions {

    AcbWalkOptions();

    HCA_DECODER_CONFIG decoderConfig;
    bool_t useCueName;
    AcbWalkCallback callback;

    ACB_EXTRACT_DISCOVERY discovery;
    bool_t prependId;

};

int AcbWalk(const std::string &inputAcbFile, AcbWalkOptions *options);
