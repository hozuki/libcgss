#include <stdio.h>
#include <stdint.h>
#include <string.h>

#if defined(_WIN32) || defined(WIN32)

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "../../lib/cgss_api.h"

using namespace cgss;

void print_help();

void MakeDirectories(const std::string &s);

std::string GetDirectoryFromPath(const std::string &s);

std::string GetFileNameFromPath(const std::string &s);

void CopyStream(IStream *src, IStream *dst);

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

    CFileStream fileStream(filePath, FileMode::OpenExisting, FileAccess::Read);
    CAcbFile acb(&fileStream, filePath);

    acb.Initialize();

    const std::string fileDir = GetDirectoryFromPath(filePath);
    const std::string extractDir = fileDir + "/_acb_" + GetFileNameFromPath(filePath) + "/";

    MakeDirectories(extractDir);

    const auto &fileNames = acb.GetFileNames();

    uint32_t i = 0;

    for (const auto &fileName : fileNames) {
        auto s = fileName;
        auto isCueNonEmpty = !s.empty();

        if (!isCueNonEmpty) {
            s = CAcbFile::GetSymbolicFileNameFromCueId(i);
        }

        auto extractPath = extractDir + s;

        IStream *stream;

        if (isCueNonEmpty) {
            stream = acb.OpenDataStream(s.c_str());
        } else {
            stream = acb.OpenDataStream(i);
        }

        if (stream) {
            CFileStream fs(extractPath.c_str(), FileMode::Create, FileAccess::Write);
            CopyStream(stream, &fs);
        } else {
            fprintf(stderr, "Cue #%u (%s) cannot be retrieved.\n", i + 1, s.c_str());
        }

        delete stream;

        ++i;
    }

    return 0;
}

void print_help() {
    fprintf(stderr, "Usage:\n\n\tacbunpack <file>\n");
}

std::string GetDirectoryFromPath(const std::string &s) {
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

std::string GetFileNameFromPath(const std::string &s) {
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

void CopyStream(IStream *src, IStream *dst) {
    const size_t bufferSize = 10240;
    uint8_t
    buffer[bufferSize] = {0};
    uint32_t read = 1;

    while (read > 0) {
        read = src->Read(buffer, bufferSize, 0, bufferSize);

        if (read > 0) {
            dst->Write(buffer, bufferSize, 0, read);
        }

        if (read < bufferSize) {
            break;
        }
    }
}

#if defined(WIN32) || defined(_WIN32)

void MakeDirectories(const std::string &s) {
    CreateDirectory(s.c_str(), nullptr);
}

#else

void MakeDirectories(const std::string &s) {
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
