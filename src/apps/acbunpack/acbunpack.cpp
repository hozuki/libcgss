#include <cstdio>
#include <string>

#include "../../lib/cgss_api.h"
#include "../common/common.h"
#include "../common/acbextract.h"

using namespace std;
using namespace cgss;

struct AcbUnpackOptions {
    bool_t useCueName;
};

static void PrintHelp();

static int ParseArgs(int argc, const char *argv[], string &inputFile, AcbUnpackOptions &options);

static int DoWork(const string &inputFile, const AcbUnpackOptions &options);

static int ExtractFile(AcbWalkCallbackParams *params);

int main(int argc, const char *argv[]) {
    string inputFile;
    AcbUnpackOptions options = {0};

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

static int DoWork(const string &inputFile, const AcbUnpackOptions &options) {
    AcbWalkOptions o;
    o.useCueName = options.useCueName;
    o.callback = ExtractFile;

    return AcbWalk(inputFile, &o);
}

static int ExtractFile(AcbWalkCallbackParams *params) {
    fprintf(stdout, "Unpacking: %s\n", params->extractPathHint.c_str());

    CFileStream fs(params->extractPathHint.c_str(), FileMode::Create, FileAccess::Write);
    common_utils::CopyStream(params->entryDataStream, &fs);

    return 0;
}

static void PrintHelp() {
    static const char *helpMessage = "Usage:\n"
                                     "\n"
                                     "\tacbunpack <file> [-n]\n"
                                     "\n"
                                     "\t-n Use cue names for output waveforms\n";
    fprintf(stderr, "%s", helpMessage);
}

static int ParseArgs(int argc, const char *argv[], string &inputFile, AcbUnpackOptions &options) {
    if (argc < 2) {
        PrintHelp();
        return -1;
    }

    inputFile = argv[1];

    for (int i = 2; i < argc; ++i) {
        if (argv[i][0] == '-' || argv[i][0] == '/') {
            switch (argv[i][1]) {
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
