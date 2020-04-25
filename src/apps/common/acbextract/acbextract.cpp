#include <cinttypes>
#include <unordered_set>
#include <stdexcept>

#include "../acbextract.h"
#include "methods.h"

using namespace std;
using namespace cgss;

AcbWalkCallbackParams::AcbWalkCallbackParams()
    : walkOptions(nullptr), acb(nullptr), formatVersion(0),
      archive(nullptr), archiveDataStream(nullptr), entryDataStream(nullptr),
      isInternal(FALSE) {
}

AcbWalkOptions::AcbWalkOptions()
    : decoderConfig({}), useCueName(FALSE), discovery(ACB_EXTRACT_DISCOVER_DEFAULT), prependId(FALSE), callback(nullptr) {
}

static int ProcessAllBinaries(CAcbFile &acb, uint32_t formatVersion, AcbWalkOptions *options, const string &extractDir, const CAfs2Archive *archive, IStream *archiveDataStream, bool_t isInternal) {
    if (!CFileSystem::DirectoryExists(extractDir)) {
        if (!CFileSystem::MkDir(extractDir)) {
            fprintf(stderr, "Failed to create directory '%s.\n", extractDir.c_str());
            return -1;
        }
    }

    // Set up key modifier
    uint16_t keyModifier;

    if (formatVersion >= CAcbFile::KEY_MODIFIER_ENABLED_VERSION) {
        keyModifier = archive->GetHcaKeyModifier();
    } else {
        keyModifier = 0;
    }

    if (options->callback) {
        AcbWalkCallbackParams p;

        p.acb = &acb;
        p.archive = archive;
        p.archiveDataStream = archiveDataStream;
        p.formatVersion = formatVersion;
        p.isInternal = isInternal;
        p.walkOptions = options;

        p.walkOptions->decoderConfig.cipherConfig.keyModifier = keyModifier;

        switch (options->discovery) {
            case ACB_EXTRACT_DISCOVER_DEFAULT:
                ExtractFilesWithCueInfo(p, acb, options, extractDir, archive, archiveDataStream);
                break;
            case ACB_EXTRACT_DISCOVER_BY_TRACK:
                ExtractFilesWithTrackInfo(p, acb, options, extractDir);
                break;
            case ACB_EXTRACT_DISCOVER_BY_CUE:
                ExtractFilesWithCueAndSequenceInfo(p, acb, options, extractDir);
                break;
            default:
                throw std::invalid_argument("Invalid discovery method.");
        }
    }

    options->decoderConfig.cipherConfig.keyModifier = 0;

    return 0;
}

int AcbWalk(const std::string &inputAcbFile, AcbWalkOptions *options) {
    const auto baseExtractDirPath = CPath::Combine(CPath::GetDirectoryName(inputAcbFile), "_acb_" + CPath::GetFileName(inputAcbFile));

    CFileStream fileStream(inputAcbFile.c_str(), FileMode::OpenExisting, FileAccess::Read);
    CAcbFile acb(&fileStream, inputAcbFile.c_str());

    acb.Initialize();

    const CAfs2Archive *archive;
    const uint32_t formatVersion = acb.GetFormatVersion();
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
            r = ProcessAllBinaries(acb, formatVersion, options, extractDir, archive, acb.GetStream(), TRUE);
        } catch (CException &ex) {
            fprintf(stderr, "%s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
            r = -1;
        }

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

            r = ProcessAllBinaries(acb, formatVersion, options, extractDir, archive, &fs, FALSE);
        } catch (CException &ex) {
            fprintf(stderr, "%s (%d)\n", ex.GetExceptionMessage().c_str(), ex.GetOpResult());
            r = -1;
        }

        if (r != 0) {
            return r;
        }
    }

    return 0;
}
