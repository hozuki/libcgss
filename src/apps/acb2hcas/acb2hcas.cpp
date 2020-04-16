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
    union {
        struct {
            uint32_t key2;
            uint32_t key1;
        } keyParts;
        uint64_t key;
    };
    bool_t useCueName;
};

static void PrintHelp();

static int ParseArgs(int argc, const char *argv[], string &inputFile, Acb2HcasOptions &options);

static int DoWork(const string &inputFile, const Acb2HcasOptions &options);

static int ProcessHca(AcbWalkCallbackParams *params);

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier = 0);

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

static void PrintHelp() {
    cout << "Usage:\n" << endl;
    cout << "acb2hcas <acb file> [-a <key1> -b <key2>] [-k <key>] [-n]" << endl << endl;
    cout << "\t-a\tKey, lower 32 bits (in hexadecimal)" << endl;
    cout << "\t-b\tKey, higher 32 bits (in hexadecimal)" << endl;
    cout << "\t-k\tKey, 64 bits (in hexadecimal)" << endl;
    cout << "\t-n\tUse cue names for output waveforms" << endl;
}

static int ParseArgs(int argc, const char *argv[], string &inputFile, Acb2HcasOptions &options) {
    if (argc < 2) {
        PrintHelp();
        return -1;
    }

    inputFile = argv[1];

    for (int i = 2; i < argc; ++i) {
        if (argv[i][0] == '-' || argv[i][0] == '/') {
            switch (argv[i][1]) {
                case 'a':
                    if (i + 1 < argc) {
                        options.keyParts.key1 = atoh<uint32_t>(argv[++i]);
                    }
                    break;
                case 'b':
                    if (i + 1 < argc) {
                        options.keyParts.key2 = atoh<uint32_t>(argv[++i]);
                    }
                    break;
                case 'k':
                    if (i + 1 < argc) {
                        options.key = atoh<uint64_t>(argv[++i]);
                    }
                    break;
                case 'n':
                    options.useCueName = TRUE;
                    break;
                default:
                    fprintf(stderr, "Unknown option: %s\n", argv[i]);
                    return 2;
            }
        }
    }

    return 0;
}

static int DoWork(const string &inputFile, const Acb2HcasOptions &options) {
    AcbWalkOptions o;

    o.callback = ProcessHca;
    o.decoderConfig.cipherConfig.key = options.key;
    o.useCueName = options.useCueName;

    return AcbWalk(inputFile, &o);
}

static int ProcessHca(AcbWalkCallbackParams *params) {
    const auto afsSourceDesc = params->isInternal ? "internal" : "external";
    const auto isHca = CHcaFormatReader::IsPossibleHcaStream(params->entryDataStream);

    fprintf(stdout, "Processing %s AFS: #%" PRIu32 " (offset=%" PRIu32 ", size=%" PRIu32 ")",
            afsSourceDesc, (uint32_t)params->cueInfo.id, (uint32_t)params->cueInfo.offset, (uint32_t)params->cueInfo.size);

    if (isHca) {
        std::string extractFilePath = common_utils::ReplaceAnyExtension(params->extractPathHint, ".hca");
        std::string hcaKeyFilePath = common_utils::ReplaceAnyExtension(params->extractPathHint, ".hcakey");

        fprintf(stdout, " to %s...\n", extractFilePath.c_str());

        try {
            CFileStream hcaFile(extractFilePath.c_str(), FileMode::Create, FileAccess::Write);

            common_utils::CopyStream(params->entryDataStream, &hcaFile);

            WriteHcaKeyFile(hcaKeyFilePath, params->walkOptions->decoderConfig.cipherConfig.key, params->walkOptions->decoderConfig.cipherConfig.keyModifier);

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
