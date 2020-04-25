#include <string>
#include <cinttypes>

#include "../cgss_cenum.h"
#include "CAfs2Archive.h"
#include "../cgss_cdata.h"
#include "../common/type_traits.h"
#include "../takamori/exceptions/CInvalidOperationException.h"
#include "../takamori/exceptions/CFormatException.h"
#include "../takamori/exceptions/CArgumentException.h"
#include "../takamori/streams/CBinaryReader.h"
#include "../takamori/streams/CFileStream.h"
#include "../takamori/CFileSystem.h"
#include "../takamori/streams/CMemoryStream.h"
#include "../takamori/CPath.h"
#include "CAcbHelper.h"
#include "CAcbFile.h"

using namespace cgss;
using namespace std;

#define DEFAULT_BINARY_FILE_EXTENSION ".bin"
static const string DefaultBinaryFileExtension(DEFAULT_BINARY_FILE_EXTENSION);

static string GetExtensionForEncodeType(uint8_t encodeType);

template<typename T, typename TNumber = typename std::enable_if<is_numeric<T>::value, T>::type>
bool_t GetFieldValueAsNumber(CUtfTable *table, uint32_t rowIndex, const char *fieldName, T *result);

bool_t GetFieldValueAsString(CUtfTable *table, uint32_t rowIndex, const char *fieldName, string &s);

const uint32_t CAcbFile::KEY_MODIFIER_ENABLED_VERSION = 0x01300000;

CAcbFile::CAcbFile(cgss::IStream *stream, const char *fileName)
    : MyClass(stream, 0, fileName) {
}

CAcbFile::CAcbFile(IStream *stream, uint64_t streamOffset, const char *fileName)
    : MyBase(stream, streamOffset) {
    _internalAwb = nullptr;
    _externalAwb = nullptr;
    _fileName = fileName;
    _formatVersion = 0;
}

CAcbFile::~CAcbFile() {
    for (const auto &pair : _tables) {
        delete pair.second;
    }

    delete _internalAwb;
    _internalAwb = nullptr;
    delete _externalAwb;
    _externalAwb = nullptr;
}

void CAcbFile::Initialize() {
    MyBase::Initialize();

    GetFieldValueAsNumber(this, 0, "Version", &_formatVersion);

    InitializeCueList();
    InitializeCueNameToWaveformMap();
    InitializeTrackList();
    InitializeAwbArchives();
}

void CAcbFile::InitializeCueList() {
    auto cueTable = GetTable("CueTable");

    if (cueTable == nullptr) {
        throw CFormatException("Missing 'Cue' table.");
    }

    auto waveformTable = GetTable("WaveformTable");

    if (waveformTable == nullptr) {
        throw CFormatException("Missing 'Waveform' table.");
    }

    auto synthTable = GetTable("SynthTable");

    if (synthTable == nullptr) {
        throw CFormatException("Missing 'Synth' table.");
    }

    const auto cueCount = cueTable->GetRows().size();

    uint64_t refItemOffset = 0;
    uint32_t refItemSize = 0;
    uint32_t refSizeCorrection = 0;

    auto &cues = _cues;

    cues.reserve(cueCount);

    for (uint32_t i = 0; i < cueCount; ++i) {
        ACB_CUE_RECORD cue = {0};

        cue.isWaveformIdentified = FALSE;
        GetFieldValueAsNumber(cueTable, i, "CueId", &cue.cueId);
        GetFieldValueAsNumber(cueTable, i, "ReferenceType", &cue.referenceType);
        GetFieldValueAsNumber(cueTable, i, "ReferenceIndex", &cue.referenceIndex);

        switch (cue.referenceType) {
            case 2:
                synthTable->GetFieldOffset(cue.referenceIndex, "ReferenceItems", &refItemOffset);
                synthTable->GetFieldSize(cue.referenceIndex, "ReferenceItems", &refItemSize);
                refSizeCorrection = refItemSize + 2;
                break;
            case 3:
            case 8:
                if (i == 0) {
                    synthTable->GetFieldOffset(0, "ReferenceItems", &refItemOffset);
                    synthTable->GetFieldSize(0, "ReferenceItems", &refItemSize);
                    refSizeCorrection = refItemSize - 2;
                } else {
                    refSizeCorrection += 4;
                }
                break;
            default:
                throw CFormatException("Unknown reference type.");
        }

        if (refItemSize != 0) {
            CBinaryReader reader(GetStream());

            cue.waveformIndex = reader.PeekUInt16BE(refItemOffset + refSizeCorrection);

            uint8_t isStreaming;
            auto hasIsStreaming = GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "Streaming", &isStreaming);

            if (hasIsStreaming) {
                cue.isStreaming = isStreaming;

                uint16_t waveformId;

                if (GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "Id", &waveformId)) {
                    cue.waveformId = waveformId;
                } else {
                    if (cue.isStreaming) {
                        if (GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "StreamAwbId", &waveformId)) {
                            cue.waveformId = waveformId;
                        }
                    } else {
                        if (GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "MemoryAwbId", &waveformId)) {
                            cue.waveformId = waveformId;
                        }
                    }
                }

                uint8_t encodeType;
                if (GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "EncodeType", &encodeType)) {
                    cue.encodeType = encodeType;
                }

                cue.isWaveformIdentified = TRUE;
            }
        }

        cues.push_back(cue);
    }
}

void CAcbFile::InitializeCueNameToWaveformMap() {
    auto cueNameTable = GetTable("CueNameTable");

    if (cueNameTable == nullptr) {
        throw CFormatException("Missing 'CueName' table.");
    }

    auto &cues = _cues;

    auto cueNameCount = cueNameTable->GetRows().size();
    for (uint32_t i = 0; i < cueNameCount; ++i) {
        uint16_t cueIndex;

        if (!GetFieldValueAsNumber(cueNameTable, i, "CueIndex", &cueIndex)) {
            continue;
        }

        auto &cue = _cues[cueIndex];

        if (cue.isWaveformIdentified) {
            string cueName;

            if (!GetFieldValueAsString(cueNameTable, i, "CueName", cueName)) {
                continue;
            }

            cueName += GetExtensionForEncodeType(cue.encodeType);

            snprintf(cue.cueName, ACB_CUE_RECORD_NAME_MAX_LEN, "%s", cueName.c_str());

            _cueNameToWaveform[cueName] = cue.waveformId;

            _fileNames.push_back(cue.cueName);
        }
    }
}

// TODO: remove duplicate code (see InitializeCueList())
void CAcbFile::InitializeTrackList() {
    auto trackTable = GetTable("TrackTable");

    if (trackTable == nullptr) {
        throw CFormatException("Missing 'Track' table.");
    }

    auto waveformTable = GetTable("WaveformTable");

    if (waveformTable == nullptr) {
        throw CFormatException("Missing 'Waveform' table.");
    }

    auto synthTable = GetTable("SynthTable");

    if (synthTable == nullptr) {
        throw CFormatException("Missing 'Synth' table.");
    }

    const auto trackCount = trackTable->GetRows().size();
    const auto &synthRows = synthTable->GetRows();
    const auto synthCount = synthRows.size();

    if (trackCount != synthCount) {
        throw CFormatException("Number of tracks and number of synthesis records do not match.");
    }

    uint64_t refItemOffset = 0;
    uint32_t refItemSize = 0;
    uint32_t refSizeCorrection = 0;

    CBinaryReader reader(GetStream());
    auto &tracks = _tracks;

    tracks.reserve(trackCount);

    for (uint32_t i = 0; i < trackCount; ++i) {
        ACB_TRACK_RECORD track = {0};

        track.isWaveformIdentified = FALSE;
        track.trackIndex = i;
        track.synthIndex = track.trackIndex;

        const auto &synthRow = synthRows[i];
        UTF_FIELD *refItemField = nullptr;

        for (const auto &field : synthRow.fields) {
            if (strcmp(field->name, "ReferenceItems") == 0) {
                refItemField = field;
                break;
            }
        }

        if (refItemField == nullptr) {
            throw CFormatException("Missing 'ReferenceItems' field in row.");
        }

        bool isStoredPerRow;

        switch (refItemField->storage) {
            case CGSS_UTF_COLUMN_STORAGE_PER_ROW:
                isStoredPerRow = true;
                break;
            case CGSS_UTF_COLUMN_STORAGE_CONST:
            case CGSS_UTF_COLUMN_STORAGE_CONST2:
                isStoredPerRow = false;
                break;
            default:
                throw CFormatException("Unsupported field storage type for 'ReferenceItems' field.");
        }

        if (isStoredPerRow) {
            synthTable->GetFieldOffset(track.synthIndex, "ReferenceItems", &refItemOffset);
            synthTable->GetFieldSize(track.synthIndex, "ReferenceItems", &refItemSize);
            // TODO: I think this should be -2 instead of +2 when looping through synth records.
            refSizeCorrection = refItemSize - 2;
        } else {
            if (i == 0) {
                synthTable->GetFieldOffset(0, "ReferenceItems", &refItemOffset);
                synthTable->GetFieldSize(0, "ReferenceItems", &refItemSize);
                refSizeCorrection = refItemSize - 2;
            } else {
                refSizeCorrection += 4;
            }
        }

        if (refItemSize != 0) {
            track.waveformIndex = reader.PeekUInt16BE(refItemOffset + refSizeCorrection);

            uint8_t isStreaming;
            auto hasIsStreaming = GetFieldValueAsNumber(waveformTable, track.waveformIndex, "Streaming", &isStreaming);

            if (hasIsStreaming) {
                track.isStreaming = isStreaming;

                uint16_t waveformId;

                if (GetFieldValueAsNumber(waveformTable, track.waveformIndex, "Id", &waveformId)) {
                    track.waveformId = waveformId;
                } else {
                    if (track.isStreaming) {
                        if (GetFieldValueAsNumber(waveformTable, track.waveformIndex, "StreamAwbId", &waveformId)) {
                            track.waveformId = waveformId;
                        }
                    } else {
                        if (GetFieldValueAsNumber(waveformTable, track.waveformIndex, "MemoryAwbId", &waveformId)) {
                            track.waveformId = waveformId;
                        }
                    }
                }

                uint8_t encodeType;
                if (GetFieldValueAsNumber(waveformTable, track.waveformIndex, "EncodeType", &encodeType)) {
                    track.encodeType = encodeType;
                }

                track.isWaveformIdentified = TRUE;
            }
        }

        tracks.push_back(track);
    }
}

void CAcbFile::InitializeAwbArchives() {
    uint32_t internalAwbSize;
    if (GetFieldSize(0, "AwbFile", &internalAwbSize) && internalAwbSize > 0) {
        _internalAwb = GetInternalAwb();
    }

    uint32_t externalAwbSize;
    if (GetFieldSize(0, "StreamAwbAfs2Header", &externalAwbSize) && externalAwbSize > 0) {
        _externalAwb = GetExternalAwb();
    }
}

CUtfTable *CAcbFile::GetTable(const char *tableName) const {
    string s(tableName);

    CUtfTable *table;

    if (_tables.find(s) != _tables.end()) {
        table = _tables[s];
    } else {
        table = ResolveTable(tableName);

        if (table) {
            _tables[s] = table;
        }
    }

    return table;
}

CUtfTable *CAcbFile::ResolveTable(const char *tableName) const {
    uint64_t tableOffset;

    if (!GetFieldOffset(0, tableName, &tableOffset)) {
        return nullptr;
    }

    uint32_t tableSize;

    if (!GetFieldSize(0, tableName, &tableSize)) {
        return nullptr;
    }

    auto tbl = new CUtfTable(GetStream(), tableOffset);

    return tbl;
}

const CAfs2Archive *CAcbFile::GetInternalAwb() const {
    uint64_t internalAwbOffset;

    if (!GetFieldOffset(0, "AwbFile", &internalAwbOffset) || internalAwbOffset == 0) {
        return nullptr;
    }

    uint32_t internalAwbSize;

    if (!GetFieldSize(0, "AwbFile", &internalAwbSize) || internalAwbSize == 0) {
        return nullptr;
    }

    auto internalAwb = new CAfs2Archive(GetStream(), internalAwbOffset, GetFileName(), FALSE);

    return internalAwb;
}

const CAfs2Archive *CAcbFile::GetExternalAwb() const {
    const auto extAwbFileName = FindExternalAwbFileName();

    if (extAwbFileName.empty()) {
        return nullptr;
    }

    // Checksum checking is skipped.

    const auto fs = new CFileStream(extAwbFileName.c_str(), FileMode::OpenExisting, FileAccess::Read);
    const auto archive = new CAfs2Archive(fs, 0, extAwbFileName.c_str(), TRUE);

    return archive;
}

const vector<string> &CAcbFile::GetFileNames() const {
    return _fileNames;
}

IStream *CAcbFile::OpenDataStream(const AFS2_FILE_RECORD *fileRecord, bool_t isStreaming) const {
    IStream *stream;

    if (isStreaming) {
        if (_externalAwb) {
            stream = _externalAwb->GetStream();
        } else {
            stream = nullptr;
        }
    } else {
        stream = this->GetStream();
    }

    auto result = CAcbHelper::ExtractToNewStream(stream, fileRecord->fileOffsetAligned, static_cast<uint32_t>(fileRecord->fileSize));

    return result;
}

IStream *CAcbFile::OpenDataStream(const char *fileName) const {
    IStream *result = nullptr;

    const auto cue = GetCueRecordByWaveformFileName(fileName);

    if (cue) {
        const auto file = GetFileRecordByCueRecord(cue);
        const auto stream = ChooseSourceStream(cue);

        result = CAcbHelper::ExtractToNewStream(stream, file->fileOffsetAligned, static_cast<uint32_t>(file->fileSize));
    }

    return result;
}

IStream *CAcbFile::OpenDataStream(uint32_t cueId) const {
    IStream *result = nullptr;

    const auto cue = GetCueRecordByCueId(cueId);

    if (cue) {
        const auto file = GetFileRecordByCueRecord(cue);
        const auto stream = ChooseSourceStream(cue);

        result = CAcbHelper::ExtractToNewStream(stream, file->fileOffsetAligned, static_cast<uint32_t>(file->fileSize));
    }

    return result;
}

const char *CAcbFile::GetFileName() const {
    return _fileName;
}

string CAcbFile::GetSymbolicFileNameHintByCueId(uint32_t cueId) const {
    auto baseName = GetSymbolicFileBaseNameByCueId(cueId);

    const auto extHint = GetFileExtensionHintByCueId(cueId);
    const auto &ext = extHint.empty() ? DefaultBinaryFileExtension : extHint;

    return baseName + ext;
}

std::string CAcbFile::GetSymbolicFileBaseNameByCueId(uint32_t cueId) {
    char buffer[256] = {0};

    sprintf(buffer, "cue_%06" PRIu32, cueId);

    return string(buffer);
}

std::string CAcbFile::GetSymbolicFileNameHintByTrackIndex(uint32_t trackIndex) const {
    auto baseName = GetSymbolicFileBaseNameByTrackIndex(trackIndex);

    const auto extHint = GetFileExtensionHintByTrackIndex(trackIndex);
    const auto &ext = extHint.empty() ? DefaultBinaryFileExtension : extHint;

    return baseName + ext;
}

std::string CAcbFile::GetSymbolicFileBaseNameByTrackIndex(uint32_t trackIndex) {
    char buffer[256] = {0};

    sprintf(buffer, "track_%06" PRIu32, trackIndex);

    return string(buffer);
}

string CAcbFile::GetCueNameByCueId(uint32_t cueId) const {
    for (const auto &cue : _cues) {
        if (cue.waveformId == cueId) {
            return string(cue.cueName);
        }
    }

    return GetSymbolicFileNameHintByCueId(cueId);
}

std::string CAcbFile::GetCueNameByTrackIndex(uint32_t trackIndex) const {
    auto track = GetTrackRecordByTrackIndex(trackIndex);

    if (track != nullptr) {
        for (const auto &cue : _cues) {
            if (cue.waveformId == track->waveformId) {
                return string(cue.cueName);
            }
        }
    }

    return GetSymbolicFileNameHintByTrackIndex(trackIndex);
}

const ACB_CUE_RECORD *CAcbFile::GetCueRecordByWaveformFileName(const char *waveformFileName) const {
    for (const auto &cue : _cues) {
        if (strcmp(cue.cueName, waveformFileName) == 0) {
            return &cue;
        }
    }

    return nullptr;
}

const ACB_CUE_RECORD *CAcbFile::GetCueRecordByCueId(uint32_t cueId) const {
    for (const auto &cue : _cues) {
        if (cue.cueId == cueId) {
            return &cue;
        }
    }

    return nullptr;
}

const AFS2_FILE_RECORD *CAcbFile::GetFileRecordByWaveformFileName(const char *waveformFileName) const {
    const auto cue = GetCueRecordByWaveformFileName(waveformFileName);

    if (cue == nullptr) {
        return nullptr;
    }

    return GetFileRecordByCueRecord(cue);
}

const AFS2_FILE_RECORD *CAcbFile::GetFileRecordByCueId(uint32_t cueId) const {
    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return nullptr;
    }

    return GetFileRecordByCueRecord(cue);
}

const AFS2_FILE_RECORD *CAcbFile::GetFileRecordByTrackIndex(uint32_t trackIndex) const {
    const ACB_TRACK_RECORD *track = nullptr;

    for (const auto &trackRecord : _tracks) {
        if (trackRecord.trackIndex == trackIndex) {
            track = &trackRecord;
            break;
        }
    }

    if (track == nullptr) {
        return nullptr;
    }

    // TODO: remove duplicate code (see GetFileRecordByCueRecord())
    if (!track->isWaveformIdentified) {
        return nullptr;
    }

    if (track->isStreaming) {
        auto externalAwb = _externalAwb;

        if (externalAwb == nullptr) {
            return nullptr;
        }

        auto &files = externalAwb->GetFiles();
        if (files.find(track->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(track->waveformId);

        return &file;
    } else {
        auto internalAwb = _internalAwb;

        if (internalAwb == nullptr) {
            return nullptr;
        }

        auto &files = internalAwb->GetFiles();

        if (files.find(track->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(track->waveformId);

        return &file;
    }
}

uint32_t CAcbFile::GetTrackCountOfCueByCueId(uint32_t cueId) const {
    auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return 0;
    }

    auto sequenceTable = GetTable("SequenceTable");

    if (sequenceTable == nullptr) {
        throw CFormatException("Missing 'Sequence' table.");
    }

    uint16_t numTracks = 0;

    GetFieldValueAsNumber(sequenceTable, cue->referenceIndex, "NumTracks", &numTracks);

    return numTracks;
}

bool_t CAcbFile::GetTrackIndicesOfCueByCueId(uint32_t cueId, uint32_t *numberOfTracks, uint32_t *trackIndices) const {
    if (trackIndices == nullptr) {
        if (numberOfTracks != nullptr) {
            *numberOfTracks = GetTrackCountOfCueByCueId(cueId);
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (numberOfTracks == nullptr) {
        throw CArgumentException("Pointer to number of tracks cannot be null.");
    }

    const auto givenNumberOfTracks = *numberOfTracks;
    *numberOfTracks = 0;

    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return FALSE;
    }

    const auto sequenceTable = GetTable("SequenceTable");

    if (sequenceTable == nullptr) {
        throw CFormatException("Missing 'Sequence' table.");
    }

    const auto expectedNumberOfTracks = GetTrackCountOfCueByCueId(cueId);

    *numberOfTracks = expectedNumberOfTracks;

    if (givenNumberOfTracks < expectedNumberOfTracks) {
        return FALSE;
    }

    uint64_t trackIndexOffset = 0;
    uint32_t trackIndexSize = 0;
    sequenceTable->GetFieldOffset(cue->referenceIndex, "TrackIndex", &trackIndexOffset);
    sequenceTable->GetFieldSize(cue->referenceIndex, "TrackIndex", &trackIndexSize);

    if (trackIndexSize != expectedNumberOfTracks * sizeof(uint16_t)) {
        throw CFormatException("Track index data error: data size and declared count does not match.");
    }

    CBinaryReader reader(GetStream());

    for (uint32_t i = 0; i < expectedNumberOfTracks; i += 1) {
        auto trackIndex = reader.PeekUInt16BE(trackIndexOffset + sizeof(uint16_t) * i);
        trackIndices[i] = trackIndex;
    }

    return TRUE;
}

bool_t CAcbFile::GetTrackIndicesOfCueByCueId(uint32_t cueId, std::vector<uint32_t> &trackIndices) const {
    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return FALSE;
    }

    const auto sequenceTable = GetTable("SequenceTable");

    if (sequenceTable == nullptr) {
        throw CFormatException("Missing 'Sequence' table.");
    }

    const auto expectedNumberOfTracks = GetTrackCountOfCueByCueId(cueId);

    uint64_t trackIndexOffset = 0;
    uint32_t trackIndexSize = 0;
    sequenceTable->GetFieldOffset(cue->referenceIndex, "TrackIndex", &trackIndexOffset);
    sequenceTable->GetFieldSize(cue->referenceIndex, "TrackIndex", &trackIndexSize);

    if (trackIndexSize != expectedNumberOfTracks * sizeof(uint16_t)) {
        throw CFormatException("Track index data error: data size and declared count does not match.");
    }

    trackIndices.resize(expectedNumberOfTracks);

    CBinaryReader reader(GetStream());

    for (uint32_t i = 0; i < expectedNumberOfTracks; i += 1) {
        auto trackIndex = reader.PeekUInt16BE(trackIndexOffset + sizeof(uint16_t) * i);
        trackIndices[i] = trackIndex;
    }

    return TRUE;
}

const std::vector<ACB_TRACK_RECORD> &CAcbFile::GetTrackRecords() const {
    return _tracks;
}

bool_t CAcbFile::IsCueIdentified(uint32_t cueId) const {
    for (auto &cue: _cues) {
        if (cue.waveformId == cueId) {
            return cue.isWaveformIdentified;
        }
    }

    return FALSE;
}

uint32_t CAcbFile::GetFormatVersion() const {
    return _formatVersion;
}

std::string CAcbFile::GetFileExtensionHintByCueId(uint32_t cueId) const {
    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return std::string();
    } else {
        return GetExtensionForEncodeType(cue->encodeType);
    }
}

std::string CAcbFile::GetFileExtensionHintByWaveformFileName(const char *waveformFileName) const {
    const auto cue = GetCueRecordByWaveformFileName(waveformFileName);

    if (cue == nullptr) {
        return std::string();
    } else {
        return GetExtensionForEncodeType(cue->encodeType);
    }
}

std::string CAcbFile::GetFileExtensionHintByTrackIndex(uint32_t trackIndex) const {
    auto track = GetTrackRecordByTrackIndex(trackIndex);

    if (track == nullptr) {
        return DefaultBinaryFileExtension;
    } else {
        return GetExtensionForEncodeType(track->encodeType);
    }
}

const ACB_TRACK_RECORD *CAcbFile::GetTrackRecordByTrackIndex(uint32_t trackIndex) const {
    for (const auto &track : _tracks) {
        if (track.trackIndex == trackIndex) {
            return &track;
        }
    }

    return nullptr;
}

std::string CAcbFile::FindExternalAwbFileName() const {
    const string acbFileName = _fileName;
    const auto awbDirPath = CPath::GetDirectoryName(acbFileName);

    auto awbBaseFileName = CPath::GetFileBaseName(acbFileName);

    awbBaseFileName = CPath::Combine(awbDirPath, awbBaseFileName);

    auto test = awbBaseFileName + "_streamfiles.awb";

    if (CFileSystem::FileExists(test.c_str())) {
        return test;
    }

    test = awbBaseFileName + ".awb";

    if (CFileSystem::FileExists(test.c_str())) {
        return test;
    }

    test = awbBaseFileName + "_STR.awb";

    if (CFileSystem::FileExists(test.c_str())) {
        return test;
    }

    return "";
}

const AFS2_FILE_RECORD *CAcbFile::GetFileRecordByCueRecord(const ACB_CUE_RECORD *cue) const {
    if (cue == nullptr) {
        return nullptr;
    }

    if (!cue->isWaveformIdentified) {
        return nullptr;
    }

    if (cue->isStreaming) {
        auto externalAwb = _externalAwb;

        if (externalAwb == nullptr) {
            return nullptr;
        }

        auto &files = externalAwb->GetFiles();
        if (files.find(cue->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(cue->waveformId);

        return &file;
    } else {
        auto internalAwb = _internalAwb;

        if (internalAwb == nullptr) {
            return nullptr;
        }

        auto &files = internalAwb->GetFiles();

        if (files.find(cue->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(cue->waveformId);

        return &file;
    }
}

IStream *CAcbFile::ChooseSourceStream(const ACB_CUE_RECORD *cue) const {
    if (cue == nullptr) {
        return nullptr;
    }

    if (cue->isStreaming) {
        return _externalAwb->GetStream();
    } else {
        return this->GetStream();
    }
}

static string GetExtensionForEncodeType(uint8_t encodeType) {
    auto type = static_cast<CGSS_ACB_WAVEFORM_ENCODE_TYPE>(encodeType);

    switch (type) {
        case CGSS_ACB_WAVEFORM_ADX:
            return ".adx";
        case CGSS_ACB_WAVEFORM_HCA:
        case CGSS_ACB_WAVEFORM_HCA2:
            return ".hca";
        case CGSS_ACB_WAVEFORM_VAG:
            return ".vag";
        case CGSS_ACB_WAVEFORM_ATRAC3:
            return ".at3";
        case CGSS_ACB_WAVEFORM_BCWAV:
            return ".bcwav";
        case CGSS_ACB_WAVEFORM_NINTENDO_DSP:
            return ".dsp";
        default:
            break;
    }

    char buffer[20] = {0};
    sprintf(buffer, ".et-%" PRId32 DEFAULT_BINARY_FILE_EXTENSION, static_cast<int32_t>(encodeType));

    return string(buffer);
}

template<typename T, typename TNumber>
bool_t GetFieldValueAsNumber(CUtfTable *table, uint32_t rowIndex, const char *fieldName, T *result) {
    if (result) {
        memset(result, 0, sizeof(TNumber));
    }

    auto &rows = table->GetRows();

    if (rowIndex >= rows.size()) {
        return FALSE;
    }

    auto &row = rows[rowIndex];

    for (auto &field : row.fields) {
        if (strcmp(fieldName, field->name) == 0) {
            if (result) {
                switch (field->type) {
                    case CGSS_UTF_COLUMN_TYPE_U8:
                        *result = field->value.u8;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_S8:
                        *result = field->value.s8;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_U16:
                        *result = field->value.u16;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_S16:
                        *result = field->value.s16;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_U32:
                        *result = field->value.u32;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_S32:
                        *result = field->value.s32;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_U64:
                        *result = field->value.u64;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_S64:
                        *result = field->value.s64;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_R32:
                        *result = field->value.r32;
                        break;
                    case CGSS_UTF_COLUMN_TYPE_R64:
                        *result = field->value.r64;
                        break;
                    default:
                        throw CInvalidOperationException("Unsupported field type for retrieving numeric value.");
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

bool_t GetFieldValueAsString(CUtfTable *table, uint32_t rowIndex, const char *fieldName, string &s) {
    auto &rows = table->GetRows();

    if (rowIndex >= rows.size()) {
        return FALSE;
    }

    auto &row = rows[rowIndex];

    for (auto &field : row.fields) {
        if (
            strcmp(fieldName, field->name) == 0) {
            s = string(field->value.str);

            return TRUE;
        }
    }

    return FALSE;
}
