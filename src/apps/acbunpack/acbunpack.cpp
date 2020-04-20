#include <cstdio>
#include <string>

#include "../../lib/cgss_api.h"
#include "../common/common.h"
#include "../common/acbextract.h"

using namespace std;
using namespace cgss;

struct AcbUnpackOptions {
    bool_t useCueName;
    bool_t byTrackIndex;
    bool_t prependId;
};

static void PrintAppTitle(FILE *out);

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
    PrintAppTitle(stdout);

    AcbWalkOptions o;
    o.callback = ExtractFile;
    o.useCueName = options.useCueName;
    o.byTrackIndex = options.byTrackIndex;

    return AcbWalk(inputFile, &o);
}

static int ExtractFile(AcbWalkCallbackParams *params) {
    fprintf(stdout, "Unpacking: %s\n", params->extractPathHint.c_str());

    CFileStream fs(params->extractPathHint.c_str(), FileMode::Create, FileAccess::Write);
    common_utils::CopyStream(params->entryDataStream, &fs);

    return 0;
}

static void PrintAppTitle(FILE *out) {
    static const char *appName = "acbunpack: ACB archive unpack utility";
    static const char *appVersion = "v0.1";

    fprintf(out, "%s %s\n\n", appName, appVersion);
}

static void PrintHelp() {
    PrintAppTitle(stderr);

    static const char *helpMessage = "Usage:\n"
                                     "\n"
                                     "acbunpack <file> [-n] [-byTrackIndex]\n"
                                     "\n"
                                     "\t-n\tUse cue names for output waveforms\n"
                                     "\t-byTrackIndex\tIdentify waveforms by their track indices\n"
                                     "\t-prependId\tPrepend file ID (cue ID, track index, etc.)\n";
    fprintf(stderr, "%s", helpMessage);
}

static int ParseArgs(int argc, const char *argv[], string &inputFile, AcbUnpackOptions &options) {
    if (argc < 2) {
        PrintHelp();
        return -1;
    }

    inputFile = argv[1];

    for (int i = 2; i < argc; ++i) {
        auto currentArgParsed = false;

        if (argv[i][0] == '-' || argv[i][0] == '/') {
            const char *argName = argv[i] + 1;

            if (stricmp(argName, "n") == 0) {
                options.useCueName = TRUE;
                currentArgParsed = true;
            } else if (stricmp(argName, "byTrackIndex") == 0) {
                options.byTrackIndex = TRUE;
                currentArgParsed = true;
            } else if (stricmp(argName, "prependId") == 0) {
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
