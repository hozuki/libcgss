#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "../cgssh.h"
#include "../../lib/cgss_api.h"

using namespace std;
using namespace cgss;

struct Options {
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

static int ParseArgs(int argc, const char *argv[], const char **input, Options &options);

static int DoWork(const string &inputFile, const Options &options);

int ProcessAllBinaries(CAcbFile *acb, uint32_t formatVersion, const Options &options, const string &extractDir, CAfs2Archive *archive, IStream *dataStream, bool_t isInternal);

static void CopyStream(IStream *src, IStream *dst);

static string ReplaceExtension(const std::string &s, const std::string &oldExt, const std::string &newExt);

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier = 0);

template<typename T>
T atoh(const char *str);

template<typename T>
T atoh(const char *str, int max_length);

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        PrintHelp();
        return 0;
    }

    const char *inputFile;
    Options options = {0};

    const auto parsed = ParseArgs(argc, argv, &inputFile, options);

    if (parsed < 0) {
        return 0;
    } else if (parsed > 0) {
        return parsed;
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

static int ParseArgs(int argc, const char *argv[], const char **input, Options &options) {
    if (argc < 2) {
        PrintHelp();
        return -1;
    }

    *input = argv[1];

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
                    return 2;
            }
        }
    }

    return 0;
}

static int DoWork(const string &inputFile, const Options &options) {
    const auto baseExtractDirPath = CPath::Combine(CPath::GetDirectoryName(inputFile), "_acb_" + CPath::GetFileName(inputFile));

    CFileStream fileStream(inputFile.c_str(), FileMode::OpenExisting, FileAccess::Read);
    CAcbFile acb(&fileStream, inputFile.c_str());

    acb.Initialize();

    CAfs2Archive *archive = nullptr;
    uint32_t formatVersion = acb.GetFormatVersion();
    int r;

    try {
        archive = acb.GetInternalAwb();
    } catch (CException &ex) {
        fprintf(stderr, "%s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
        archive = nullptr;
    }

    if (archive) {
        const auto extractDir = CPath::Combine(baseExtractDirPath, "internal");

        try {
            r = ProcessAllBinaries(&acb, formatVersion, options, extractDir, archive, acb.GetStream(), TRUE);
        } catch (CException &ex) {
            fprintf(stderr, "%s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
            r = -1;
        }

        delete archive;

        if (r != 0) {
            return r;
        }
    }

    try {
        archive = acb.GetExternalAwb();
    } catch (CException &ex) {
        fprintf(stderr, "%s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
        archive = nullptr;
    }

    if (archive) {
        const auto extractDir = CPath::Combine(baseExtractDirPath, "external");

        try {
            CFileStream fs(archive->GetFileName(), FileMode::OpenExisting, FileAccess::Read);

            r = ProcessAllBinaries(&acb, formatVersion, options, extractDir, archive, &fs, FALSE);
        } catch (CException &ex) {
            fprintf(stderr, "%s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
            r = -1;
        }

        delete archive;

        if (r != 0) {
            return r;
        }
    }

    return 0;
}

static int
ProcessAllBinaries(CAcbFile *acb, uint32_t formatVersion, const Options &options, const string &extractDir, CAfs2Archive *archive, IStream *dataStream, bool_t isInternal) {
    if (!CFileSystem::DirectoryExists(extractDir)) {
        if (!CFileSystem::MkDir(extractDir)) {
            fprintf(stderr, "Failed to create directory %s.\n", extractDir.c_str());
            return -1;
        }
    }

    const auto afsSource = isInternal ? "internal" : "external";

    uint16_t keyModifier;

    if (formatVersion >= CAcbFile::KEY_MODIFIER_ENABLED_VERSION) {
        keyModifier = archive->GetHcaKeyModifier();
    } else {
        keyModifier = 0;
    }

    for (auto &entry : archive->GetFiles()) {
        auto &record = entry.second;
        std::string extractFileName;
        std::string hcaKeyFileName;

        if (options.useCueName) {
            extractFileName = acb->GetCueNameFromCueId(record.cueId);
            hcaKeyFileName = ReplaceExtension(extractFileName, ".hca", ".hcakey");
        } else {
            extractFileName = CAcbFile::GetSymbolicFileNameFromCueId(record.cueId);
            hcaKeyFileName = ReplaceExtension(extractFileName, ".bin", ".hcakey");
        }

        auto extractFilePath = CPath::Combine(extractDir, extractFileName);
        auto hcaKeyFilePath = CPath::Combine(extractDir, hcaKeyFileName);

        auto fileData = CAcbHelper::ExtractToNewStream(dataStream, record.fileOffsetAligned, (uint32_t)record.fileSize);

        const auto isHca = CHcaFormatReader::IsPossibleHcaStream(fileData);

        fprintf(stdout, "Processing %s AFS: #%u (offset=%u, size=%u)...   ", afsSource, (uint32_t)record.cueId, (uint32_t)record.fileOffsetAligned, (uint32_t)record.fileSize);

        if (isHca) {
            try {
                CFileStream hcaFile(extractFilePath.c_str(), FileMode::Create, FileAccess::Write);

                CopyStream(fileData, &hcaFile);

                WriteHcaKeyFile(hcaKeyFilePath, options.key, keyModifier);

                fprintf(stdout, "exported\n");
            } catch (CException &ex) {
                if (CFileSystem::FileExists(extractFilePath)) {
                    CFileSystem::RmFile(extractFilePath);
                }

                fprintf(stdout, "errored: %s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
            }
        } else {
            fprintf(stdout, "skipped (not HCA)\n");
        }

        delete fileData;
    }

    return 0;
}

static void CopyStream(IStream *src, IStream *dst) {
    const size_t bufferSize = 10240;
    uint8_t buffer[bufferSize] = {0};
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

// https://stackoverflow.com/a/874160
static bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

static string ReplaceExtension(const std::string &s, const std::string &oldExt, const std::string &newExt) {
    if (s.size() < oldExt.size()) {
        return s;
    }

    auto sl = s;
    auto extl = oldExt;

    // ALERT!
    // Since the only usage here is replacing extensions, and we promise that the
    // extensions are in ASCII, and we don't care about chars before the extension,
    // we can use this method (tolower()). Otherwise, it causes trouble for non-
    // ASCII encodings.
    std::transform(sl.begin(), sl.end(), sl.begin(), ::tolower);
    std::transform(extl.begin(), extl.end(), extl.begin(), ::tolower);

    if (!hasEnding(sl, extl)) {
        return s;
    }

    return s.substr(0, s.size() - oldExt.size()) + newExt;
}

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier) {
    CFileStream fs(fileName.c_str(), FileMode::Create, FileAccess::Write);
    CBinaryWriter writer(&fs);

    writer.WriteUInt64BE(key);
    writer.WriteUInt16BE(modifier);
}

#define IS_NUM(ch) ('0' <= (ch) && (ch) <= '9')
#define IS_UPHEX(ch) ('A' <= (ch) && (ch) <= 'F')
#define IS_LOHEX(ch) ('a' <= (ch) && (ch) <= 'f')

template<typename T>
T atoh(const char *str) {
    return atoh<T>(str, 8);
}

template<typename T>
T atoh(const char *str, int max_length) {
    max_length = std::min((size_t)max_length, sizeof(T) * 2);

    int i = 0;
    T ret = 0;

    while (i < max_length && *str) {
        if (IS_NUM(*str)) {
            ret = (ret << 4u) | (uint32_t)(*str - '0');
        } else if (IS_UPHEX(*str)) {
            ret = (ret << 4u) | (uint32_t)(*str - 'A' + 10);
        } else if (IS_LOHEX(*str)) {
            ret = (ret << 4u) | (uint32_t)(*str - 'a' + 10);
        } else {
            break;
        }

        ++str;
    }

    return ret;
}
