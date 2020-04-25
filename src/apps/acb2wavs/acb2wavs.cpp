#include <iostream>
#include <string>
#include <algorithm>
#include <cinttypes>

#include "../cgssh.h"
#include "../../lib/cgss_api.h"
#include "../common/common.h"
#include "../common/acbextract.h"

using namespace std;
using namespace cgss;

struct Acb2WavsOptions {
    HCA_DECODER_CONFIG decoderConfig;
    bool_t useCueName;
    bool_t prependId;
    ACB_EXTRACT_DISCOVERY discovery;
};

static void PrintAppTitle(ostream &out);

static void PrintHelp();

static int ParseArgs(int argc, const char *argv[], string &inputFile, Acb2WavsOptions &options);

static int DoWork(const string &inputFile, const Acb2WavsOptions &options);

static int ProcessHca(AcbWalkCallbackParams *params);

static int DecodeHca(IStream *hcaDataStream, IStream *waveStream, const HCA_DECODER_CONFIG &dc);

int main(int argc, const char *argv[]) {
    string inputFile;
    Acb2WavsOptions options = {0};

    const auto parsed = ParseArgs(argc, argv, inputFile, options);

    if (parsed < 0) {
        return 0;
    } else if (parsed > 0) {
        return parsed;
    }

    PrintAppTitle(cout);

    if (!cgssHelperFileExists(inputFile.c_str())) {
        fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", inputFile.c_str());
        return -1;
    }

    return DoWork(inputFile, options);
}

static void PrintAppTitle(ostream &out) {
    out << "acb2wavs: ACB w/ HCA batch unpacking and decoding utility" << endl << endl;
}

static void PrintHelp() {
    PrintAppTitle(cerr);

    uint32_t k1 = 0, k2 = 0;

#if __COMPILE_WITH_CGSS_KEYS
    k1 = g_CgssKey1;
    k2 = g_CgssKey2;
#endif

    cerr << "Usage:\n" << endl;
    cerr << "acb2wavs <acb file> [-a <key1 = " << hex << k1 << ">[ [-b <key2 = " << hex << k2 << ">] [-n] [-discovery:<default|track|cue>] [-prependId]" << endl << endl;
    cerr << "\t-n\tUse cue names for output waveforms" << endl;
    cerr << "\t-discovery\tSpecify waveform discovery method (default: default/classic strategy; track: by tracks; cue: by cue group)" << endl;
    cerr << "\t-prependId\tPrepend file ID (cue ID, track index, etc.)" << endl;
}

static int ParseArgs(int argc, const char *argv[], string &inputFile, Acb2WavsOptions &options) {
    if (argc < 2) {
        PrintHelp();
        return -1;
    }

    inputFile = argv[1];

    options.decoderConfig.waveHeaderEnabled = TRUE;
    options.decoderConfig.decodeFunc = CDefaultWaveGenerator::Decode16BitS;

#if __COMPILE_WITH_CGSS_KEYS
    options.decoderConfig.cipherConfig.keyParts.key1 = g_CgssKey1;
    options.decoderConfig.cipherConfig.keyParts.key2 = g_CgssKey2;
#endif

    options.decoderConfig.cipherConfig.keyModifier = 0;

    for (int i = 2; i < argc; ++i) {
        auto currentArgParsed = false;

        constexpr size_t DiscoveryOptionPrefixLength = sizeof("discovery:") - 1;

        if (argv[i][0] == '-' || argv[i][0] == '/') {
            const char *argName = argv[i] + 1;

            if (strcmp_ignore_case(argName, "a") == 0) {
                if (i + 1 < argc) {
                    options.decoderConfig.cipherConfig.keyParts.key1 = atoh<uint32_t>(argv[++i]);
                    currentArgParsed = true;
                }
            } else if (strcmp_ignore_case(argName, "b") == 0) {
                if (i + 1 < argc) {
                    options.decoderConfig.cipherConfig.keyParts.key2 = atoh<uint32_t>(argv[++i]);
                    currentArgParsed = true;
                }
            } else if (strcmp_ignore_case(argName, "k") == 0) {
                if (i + 1 < argc) {
                    options.decoderConfig.cipherConfig.key = atoh<uint64_t>(argv[++i]);
                    currentArgParsed = true;
                }
            } else if (strcmp_ignore_case(argName, "n") == 0) {
                options.useCueName = TRUE;
                currentArgParsed = true;
            } else if (strncmp_ignore_case(argName, "discovery:", DiscoveryOptionPrefixLength) == 0) {
                if (strcmp_ignore_case(argName + DiscoveryOptionPrefixLength, "default") == 0) {
                    options.discovery = ACB_EXTRACT_DISCOVER_DEFAULT;
                    currentArgParsed = true;
                } else if (strcmp_ignore_case(argName + DiscoveryOptionPrefixLength, "track") == 0) {
                    options.discovery = ACB_EXTRACT_DISCOVER_BY_TRACK;
                    currentArgParsed = true;
                } else if (strcmp_ignore_case(argName + DiscoveryOptionPrefixLength, "cue") == 0) {
                    options.discovery = ACB_EXTRACT_DISCOVER_BY_CUE;
                    currentArgParsed = true;
                }
            } else if (strcmp_ignore_case(argName, "prependId") == 0) {
                options.prependId = TRUE;
                currentArgParsed = true;
            }

            if (currentArgParsed) {
                continue;
            }

            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 2;
        }
    }

    return 0;
}

static int DoWork(const string &inputFile, const Acb2WavsOptions &options) {
    AcbWalkOptions o;

    o.callback = ProcessHca;
    o.decoderConfig = options.decoderConfig;
    o.useCueName = options.useCueName;
    o.discovery = options.discovery;
    o.prependId = options.prependId;

    return AcbWalk(inputFile, &o);
}

static int ProcessHca(AcbWalkCallbackParams *params) {
    const auto afsSource = params->isInternal ? "internal" : "external";
    const auto isHca = CHcaFormatReader::IsPossibleHcaStream(params->entryDataStream);

    fprintf(stdout, "Processing %s AFS: #%" PRIu32 " (offset=%" PRIu32 ", size=%" PRIu32 ")",
            afsSource, (uint32_t)params->cueInfo.id, (uint32_t)params->cueInfo.offset, (uint32_t)params->cueInfo.size);

    int r;

    if (isHca) {
        HCA_DECODER_CONFIG decoderConfig = params->walkOptions->decoderConfig;

        const string extractFilePath = common_utils::ReplaceAnyExtension(params->extractPathHint, ".wav");
        fprintf(stdout, ": decoding to %s...\n", extractFilePath.c_str());

        try {
            CFileStream fs(extractFilePath.c_str(), FileMode::Create, FileAccess::Write);

            r = DecodeHca(params->entryDataStream, &fs, decoderConfig);

            if (r == 0) {
                fprintf(stdout, "decoded\n");
            } else {
                fprintf(stdout, "errored\n");
            }
        } catch (CException &ex) {
            if (CFileSystem::FileExists(extractFilePath)) {
                CFileSystem::RmFile(extractFilePath);
            }

            fprintf(stdout, "errored: %s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
        }
    } else {
        fprintf(stdout, "... skipped (not HCA)\n");
    }

    return 0;
}

static int DecodeHca(IStream *hcaDataStream, IStream *waveStream, const HCA_DECODER_CONFIG &dc) {
    CHcaDecoder decoder(hcaDataStream, dc);
    static const int bufferSize = 10240;
    uint8_t buffer[bufferSize];
    uint32_t read = 1;

    while (read > 0) {
        read = decoder.Read(buffer, bufferSize, 0, bufferSize);

        if (read > 0) {
            waveStream->Write(buffer, bufferSize, 0, read);
        }
    }

    return 0;
}
