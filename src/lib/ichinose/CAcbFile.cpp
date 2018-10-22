#include <string>
#include "../cgss_cenum.h"
#include "CAfs2Archive.h"
#include "../cgss_cdata.h"
#include "../takamori/exceptions/CInvalidOperationException.h"
#include "../takamori/exceptions/CFormatException.h"
#include "../takamori/streams/CBinaryReader.h"
#include "../takamori/streams/CFileStream.h"
#include "../takamori/CFileSystem.h"
#include "../takamori/streams/CMemoryStream.h"
#include "../takamori/CPath.h"
#include "CAcbHelper.h"
#include "CAcbFile.h"

using namespace cgss;
using namespace std;

static string GetExtensionForEncodeType(uint8_t encodeType);

template<typename T>
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
}

CAcbFile::~CAcbFile() {
    for (auto pair : _tables) {
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

    InitializeAcbTables();
    InitializeCueNameToWaveformTable();
    InitializeAwbArchives();
}

void CAcbFile::InitializeAcbTables() {
    auto cueTable = GetTable("CueTable");
    auto waveformTable = GetTable("WaveformTable");
    auto synthTable = GetTable("SynthTable");

    const auto cueCount = cueTable->GetRows().size();

    _cues.reserve(cueCount);

    uint64_t refItemOffset = 0;
    uint32_t refItemSize = 0, refCorrection = 0;
    CBinaryReader reader(GetStream());

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
                refCorrection = refItemSize + 2;
                break;
            case 3:
            case 8:
                if (i == 0) {
                    synthTable->GetFieldOffset(0, "ReferenceItems", &refItemOffset);
                    synthTable->GetFieldSize(0, "ReferenceItems", &refItemSize);
                    refCorrection = refItemSize - 2;
                } else {
                    refCorrection += 4;
                }
                break;
            default:
                throw CFormatException("Unknown reference type.");
        }

        if (refItemSize != 0) {
            cue.waveformIndex = reader.PeekUInt16BE(refItemOffset + refCorrection);

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

        _cues.push_back(cue);
    }
}

void CAcbFile::InitializeCueNameToWaveformTable() {
    auto cueNameTable = GetTable("CueNameTable");
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

            strncpy(cue.cueName, cueName.c_str(), ACB_CUE_RECORD_NAME_MAX_LEN);

            _cueNameToWaveform[cueName] = cue.waveformId;

            _fileNames.push_back(cue.cueName);
        }
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

CUtfTable *CAcbFile::GetTable(const char *tableName) {
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

CUtfTable *CAcbFile::ResolveTable(const char *tableName) {
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

CAfs2Archive *CAcbFile::GetInternalAwb() {
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

CAfs2Archive *CAcbFile::GetExternalAwb() {
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

IStream *CAcbFile::OpenDataStream(const char *fileName) {
    IStream *result = nullptr;

    for (auto &cue : _cues) {
        if (strcmp(cue.cueName, fileName) == 0) {
            result = GetDataStreamFromCueInfo(cue, fileName);
            break;
        }
    }

    return result;
}

IStream *CAcbFile::OpenDataStream(uint32_t cueId) {
    char tempFileName[40] = {0};

    sprintf(tempFileName, "cue #%u", cueId);

    IStream *result = nullptr;

    for (auto &cue : _cues) {
        if (cue.cueId == cueId) {
            result = GetDataStreamFromCueInfo(cue, tempFileName);
            break;
        }
    }

    return result;
}

IStream *CAcbFile::GetDataStreamFromCueInfo(const ACB_CUE_RECORD &cue, const char *fileNameForError) {
    if (!cue.isWaveformIdentified) {
        return nullptr;
    }

    IStream *result;

    if (cue.isStreaming) {
        auto externalAwb = _externalAwb;

        if (externalAwb == nullptr) {
            return nullptr;
        }

        auto &files = externalAwb->GetFiles();
        if (files.find(cue.waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(cue.waveformId);

        CFileStream fs(file.fileName, FileMode::OpenExisting, FileAccess::Read);

        result = CAcbHelper::ExtractToNewStream(&fs, file.fileOffsetAligned, static_cast<uint32_t>(file.fileSize));
    } else {
        auto internalAwb = _internalAwb;

        if (internalAwb == nullptr) {
            return nullptr;
        }

        auto &files = internalAwb->GetFiles();

        if (files.find(cue.waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(cue.waveformId);

        result = CAcbHelper::ExtractToNewStream(GetStream(), file.fileOffsetAligned, static_cast<uint32_t>(file.fileSize));
    }

    return result;
}

const char *CAcbFile::GetFileName() const {
    return _fileName;
}

string CAcbFile::GetSymbolicFileNameFromCueId(uint32_t cueId) {
    char buffer[40] = {0};
    sprintf(buffer, "dat_%06u.bin", cueId);
    return string(buffer);
}

string CAcbFile::GetCueNameFromCueId(uint32_t cueId) {
    for (auto &cue : _cues) {
        if (cue.waveformId == cueId) {
            return string(cue.cueName);
        }
    }
    char buffer[40] = {0};
    sprintf(buffer, "dat_%06u.hca", cueId);
    return string(buffer);
}

uint32_t CAcbFile::GetFormatVersion() const {
    return _formatVersion;
}

std::string CAcbFile::FindExternalAwbFileName() {
    const string acbFileName = _fileName;
    const auto awbDirPath = CPath::GetDirectoryName(acbFileName);

    auto awbBaseFileName = CPath::GetFileNameWithoutExtension(acbFileName);

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

static string GetExtensionForEncodeType(uint8_t encodeType) {
    auto type = static_cast<CGSS_ACB_WAVEFORM_ENCODE_TYPE>(encodeType);

    switch (type) {
        case CGSS_ACB_WAVEFORM_ADX:
            return ".adx";
        case CGSS_ACB_WAVEFORM_HCA:
            return ".hca";
        case CGSS_ACB_WAVEFORM_VAG:
            return ".vag";
        case CGSS_ACB_WAVEFORM_ATRAC3:
            return ".at3";
        case CGSS_ACB_WAVEFORM_BCWAV:
            return ".bcwav";
        case CGSS_ACB_WAVEFORM_NINTENDO_DSP:
            return ".dsp";
    }

    char buffer[20] = {0};
    sprintf(buffer, ".et-%d.bin", static_cast<int32_t>(encodeType));

    return string(buffer);
}

template<typename T>
bool_t GetFieldValueAsNumber(CUtfTable *table, uint32_t rowIndex, const char *fieldName, T *result) {
    if (result) {
        memset(result, 0, sizeof(T));
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
