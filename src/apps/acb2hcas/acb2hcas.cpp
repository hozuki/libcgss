#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "../../lib/cgss_api.h"
#include "../common/common.h"

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

static int ParseArgs(int argc, const char *argv[], const char **input, Acb2HcasOptions &options);

static int DoWork(const string &inputFile, const Acb2HcasOptions &options);

int
ProcessAllBinaries(CAcbFile *acb, uint32_t formatVersion, const Acb2HcasOptions &options, const string &extractDir, CAfs2Archive *archive, IStream *dataStream, bool_t isInternal);

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier = 0);

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        PrintHelp();
        return 0;
    }

    const char *inputFile;
    Acb2HcasOptions options = {0};

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

static int ParseArgs(int argc, const char *argv[], const char **input, Acb2HcasOptions &options) {
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

static int DoWork(const string &inputFile, const Acb2HcasOptions &options) {
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
ProcessAllBinaries(CAcbFile *acb, uint32_t formatVersion, const Acb2HcasOptions &options, const string &extractDir, CAfs2Archive *archive, IStream *dataStream, bool_t isInternal) {
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

        auto fileData = CAcbHelper::ExtractToNewStream(dataStream, record.fileOffsetAligned, (uint32_t)record.fileSize);

        const auto isHca = CHcaFormatReader::IsPossibleHcaStream(fileData);

        fprintf(stdout, "Processing %s AFS: #%u (offset=%u, size=%u)",
                afsSource, (uint32_t)record.cueId, (uint32_t)record.fileOffsetAligned, (uint32_t)record.fileSize);

        if (isHca) {
            std::string extractFileName;
            std::string hcaKeyFileName;

            if (options.useCueName) {
                extractFileName = acb->GetCueNameFromCueId(record.cueId);
            } else {
                extractFileName = CAcbFile::GetSymbolicFileNameFromCueId(record.cueId);
            }

            extractFileName = common_utils::ReplaceAnyExtension(extractFileName, ".hca");
            hcaKeyFileName = common_utils::ReplaceAnyExtension(extractFileName, ".hcakey");

            auto extractFilePath = CPath::Combine(extractDir, extractFileName);
            auto hcaKeyFilePath = CPath::Combine(extractDir, hcaKeyFileName);

            fprintf(stdout, " to %s...\n", extractFilePath.c_str());

            try {
                CFileStream hcaFile(extractFilePath.c_str(), FileMode::Create, FileAccess::Write);

                common_utils::CopyStream(fileData, &hcaFile);

                WriteHcaKeyFile(hcaKeyFilePath, options.key, keyModifier);

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

        delete fileData;
    }

    return 0;
}

static void WriteHcaKeyFile(const string &fileName, uint64_t key, uint16_t modifier) {
    CFileStream fs(fileName.c_str(), FileMode::Create, FileAccess::Write);
    CBinaryWriter writer(&fs);

    writer.WriteUInt64BE(key);
    writer.WriteUInt16BE(modifier);
}
