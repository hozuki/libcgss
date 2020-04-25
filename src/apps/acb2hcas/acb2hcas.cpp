#include <string>
#include <iostream>
#include <algorithm>
#include <cinttypes>

#include "../../lib/cgss_api.h"
#include "../common/common.h"
#include "../common/acbextract.h"

using namespace std;
using namespace cgss;

struct Acb2HcasOptions {
    HCA_CIPHER_CONFIG cipherConfig;
    bool_t useCueName;
    bool_t prependId;
    ACB_EXTRACT_DISCOVERY discovery;
};

static void PrintAppTitle(ostream &out);

static void PrintHelp();

static int ParseArgs(int argc, const char *argv[], string &inputFile, Acb2HcasOptions &options);

static int DoWork(const string &inputFile, const Acb2HcasOptions &options);

static int ProcessHca(AcbWalkCallbackParams *params);

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier);

int main(int argc, const char *argv[]) {
    string inputFile;
    Acb2HcasOptions options = {0};

    const auto parsed = ParseArgs(argc, argv, inputFile, options);

    if (parsed < 0) {
        return 0;
    } else if (parsed > 0) {
        return parsed;
    }

    if (!cgssHelperFileExists(inputFile.c_str())) {
        fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", inputFile.c_str());
        return -1;
    }

    return DoWork(inputFile, options);
}

static void PrintAppTitle(ostream &out) {
    out << "acb2hcas: ACB w/ HCA batch unpacking utility" << endl << endl;
}

static void PrintHelp() {
    PrintAppTitle(cerr);

    uint32_t k1 = 0, k2 = 0;

#if __COMPILE_WITH_CGSS_KEYS
    k1 = g_CgssKey1;
    k2 = g_CgssKey2;
#endif

    const auto key = (static_cast<uint64_t>(k2) << 32u) | static_cast<uint64_t>(k1);

    cerr << "Usage:\n" << endl;
    cerr << "acb2hcas <acb file> [-a <key1 = " << hex << k1 << ">] [-b <key2 = " << hex << k2 << ">] [-k <key = " << hex << key << ">] [-n] [-discovery:<default|track|cue>] [-prependId]" << endl << endl;
    cerr << "\t-a\tKey, lower 32 bits (in hexadecimal)" << endl;
    cerr << "\t-b\tKey, higher 32 bits (in hexadecimal)" << endl;
    cerr << "\t-k\tKey, 64 bits (in hexadecimal)" << endl;
    cerr << "\t-n\tUse cue names for output waveforms" << endl;
    cerr << "\t-discovery\tSpecify waveform discovery method (default: default/classic strategy; track: by tracks; cue: by cue group)" << endl;
    cerr << "\t-prependId\tPrepend file ID (cue ID, track index, etc.)" << endl;
}

static int ParseArgs(int argc, const char *argv[], string &inputFile, Acb2HcasOptions &options) {
    if (argc < 2) {
        PrintHelp();
        return -1;
    }

    inputFile = argv[1];

#if __COMPILE_WITH_CGSS_KEYS
    options.cipherConfig.keyParts.key1 = g_CgssKey1;
    options.cipherConfig.keyParts.key2 = g_CgssKey2;
#endif

    for (int i = 2; i < argc; ++i) {
        auto currentArgParsed = false;

        constexpr size_t DiscoveryOptionPrefixLength = sizeof("discovery:") - 1;

        if (argv[i][0] == '-' || argv[i][0] == '/') {
            const char *argName = argv[i] + 1;

            if (strcmp_ignore_case(argName, "a") == 0) {
                if (i + 1 < argc) {
                    options.cipherConfig.keyParts.key1 = atoh<uint32_t>(argv[++i]);
                    currentArgParsed = true;
                }
            } else if (strcmp_ignore_case(argName, "b") == 0) {
                if (i + 1 < argc) {
                    options.cipherConfig.keyParts.key2 = atoh<uint32_t>(argv[++i]);
                    currentArgParsed = true;
                }
            } else if (strcmp_ignore_case(argName, "k") == 0) {
                if (i + 1 < argc) {
                    options.cipherConfig.key = atoh<uint64_t>(argv[++i]);
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

static int DoWork(const string &inputFile, const Acb2HcasOptions &options) {
    PrintAppTitle(cout);

    AcbWalkOptions o;

    o.callback = ProcessHca;
    o.decoderConfig.cipherConfig.key = options.cipherConfig.key;
    o.useCueName = options.useCueName;
    o.discovery = options.discovery;
    o.prependId = options.prependId;

    return AcbWalk(inputFile, &o);
}

static int ProcessHca(AcbWalkCallbackParams *params) {
    const auto afsSourceDesc = params->isInternal ? "internal" : "external";
    const auto isHca = CHcaFormatReader::IsPossibleHcaStream(params->entryDataStream);

    fprintf(stdout, "Processing %s AFS: #%"
                    PRIu32
                    " (offset=%"
                    PRIu32
                    ", size=%"
                    PRIu32
                    ")",
            afsSourceDesc, (uint32_t)params->cueInfo.id, (uint32_t)params->cueInfo.offset, (uint32_t)params->cueInfo.size);

    if (isHca) {
        std::string extractFilePath = common_utils::ReplaceAnyExtension(params->extractPathHint, ".hca");
        std::string hcaKeyFilePath = common_utils::ReplaceAnyExtension(params->extractPathHint, ".hcakey");

        fprintf(stdout, " to %s...\n", extractFilePath.c_str());

        try {
            CFileStream hcaFile(extractFilePath.c_str(), FileMode::Create, FileAccess::Write);

            common_utils::CopyStream(params->entryDataStream, &hcaFile);

            HCA_CIPHER_CONFIG cipherConfig = params->walkOptions->decoderConfig.cipherConfig;
            WriteHcaKeyFile(hcaKeyFilePath, cipherConfig.key, cipherConfig.keyModifier);

            fprintf(stdout, "exported\n");
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

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier) {
    CFileStream fs(fileName.c_str(), FileMode::Create, FileAccess::Write);
    CBinaryWriter writer(&fs);

    writer.WriteUInt64BE(key);
    writer.WriteUInt16BE(modifier);
}
