#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cinttypes>

#if defined(_WIN32) || defined(WIN32)

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "../../lib/cgss_api.h"
#include "../common/common.h"
#include "../common/acbextract.h"

using namespace cgss;

static void print_help();

static void MakeDirectories(const std::string &s);

static std::string GetDirectoryFromPath(const std::string &s);

static std::string GetFileNameFromPath(const std::string &s);

static int ExtractFile(AcbWalkCallbackParams *params);

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    const char *filePath = argv[1];

    if (!cgssHelperFileExists(filePath)) {
        fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", filePath);
        return -1;
    }

    AcbWalkOptions options;
    options.useCueName = true;
    options.callback = ExtractFile;

    const auto r = AcbWalk(filePath, &options);

    return r;
}

static int ExtractFile(AcbWalkCallbackParams *params) {
    CFileStream fs(params->extractPathHint.c_str(), FileMode::Create, FileAccess::Write);
    common_utils::CopyStream(params->entryDataStream, &fs);

    return 0;
}

static void print_help() {
    fprintf(stderr, "Usage:\n\n\tacbunpack <file>\n");
}

static std::string GetDirectoryFromPath(const std::string &s) {
    const char sep1 = '/', sep2 = '\\';

    size_t i1 = s.rfind(sep1, s.length());
    size_t i2 = s.rfind(sep2, s.length());

    if (i1 != std::string::npos) {
        if (i2 != std::string::npos) {
            auto i = i1 > i2 ? i1 : i2;
            return (s.substr(0, i));
        } else {
            return (s.substr(0, i1));
        }
    } else {
        if (i2 != std::string::npos) {
            return (s.substr(0, i2));
        } else {
            return std::string("");
        }
    }
}

static std::string GetFileNameFromPath(const std::string &s) {
    const char sep1 = '/', sep2 = '\\';

    size_t i1 = s.rfind(sep1, s.length());
    size_t i2 = s.rfind(sep2, s.length());

    if (i1 != std::string::npos) {
        if (i2 != std::string::npos) {
            auto i = i1 > i2 ? i1 : i2;
            return (s.substr(i + 1));
        } else {
            return (s.substr(i1 + 1));
        }
    } else {
        if (i2 != std::string::npos) {
            return (s.substr(i2 + 1));
        } else {
            return std::string("");
        }
    }
}

#if defined(WIN32) || defined(_WIN32)

static void MakeDirectories(const std::string &s) {
    CreateDirectory(s.c_str(), nullptr);
}

#else

static void MakeDirectories(const std::string &s) {
    char str[512] = {0};

    strncpy(str, s.c_str(), 512);
    auto len = strlen(str);

    for (auto i = 0; i < len; i++) {
        if (str[i] == '/') {
            str[i] = '\0';
            if (access(str, 0) != 0) {
                mkdir(str, 0777);
            }
            str[i] = '/';
        }
    }

    if (len > 0 && access(str, 0) != 0) {
        mkdir(str, 0777);
    }
}

#endif
