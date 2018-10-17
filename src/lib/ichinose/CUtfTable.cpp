#include "../takamori/streams/CMemoryStream.h"
#include "../takamori/streams/CBinaryReader.h"
#include "../takamori/exceptions/CFormatException.h"
#include "../takamori/exceptions/CNotImplementedException.h"
#include "../takamori/streams/CStreamExtensions.h"
#include "CUtfTable.h"
#include "CUtfReader.h"
#include "CAcbHelper.h"
#include "CUtfField.h"
#include "../cgss_cdata.h"

CGSS_NS_BEGIN

    uint8_t UTF_SIGNATURE[] = {'@', 'U', 'T', 'F'};

    CUtfTable::CUtfTable(IStream *stream, uint64_t streamOffset)
        : _stream(stream), _streamOffset(streamOffset) {
        memset(_tableName, 0, sizeof(_tableName));
        Initialize();
    }

    CUtfTable::~CUtfTable() {
        for (auto &row : _rows) {
            for (auto pField : row.fields) {
                delete pField;
            }
        }

        if (_utfReader) {
            delete _utfReader;
            _utfReader = nullptr;
        }
    }

    IStream *CUtfTable::GetStream() const {
        return _stream;
    }

    void CUtfTable::GetHeader(UTF_HEADER &header) const {
        memcpy(&header, &_utfHeader, sizeof(UTF_HEADER));
    }

    const UTF_HEADER CUtfTable::GetHeader() const {
        return _utfHeader;
    }

    bool_t CUtfTable::IsEncrypted() const {
        return _isEncrypted;
    }

    const std::vector<CUtfTable::UtfRow> &CUtfTable::GetRows() const {
        return _rows;
    }

    const char *CUtfTable::GetName() const {
        return _tableName;
    }

    CUtfReader *CUtfTable::GetReader() const {
        return _utfReader;
    }

    void CUtfTable::Initialize() {
        auto *stream = _stream;
        const auto offset = _streamOffset;

        if (offset != stream->GetPosition()) {
            stream->Seek(offset, StreamSeekOrigin::Begin);
        }

        uint8_t magic[4];
        CBinaryReader::PeekBytes(stream, magic, 4, 0, 4);

        const auto magicFound = CheckEncryption(magic);

        if (!magicFound) {
            throw CFormatException("\"@UTF\" is not found.");
        }

        auto *tableDataStream = GetTableDataStream();
        auto &header = _utfHeader;

        ReadUtfHeader(tableDataStream, header, _tableName);

        _rows.reserve(header.rowCount);

        if (header.tableSize > 0) {
            InitializeUtfSchema(stream, tableDataStream, 0x20);
        }

        delete tableDataStream;
    }

    bool_t CUtfTable::CheckEncryption(const uint8_t *magic) {
        if (memcmp(magic, UTF_SIGNATURE, 4) == 0) {
            _utfReader = new CUtfReader();
            _isEncrypted = FALSE;
        } else {
            _isEncrypted = TRUE;
            uint8_t seed, increment;
            const auto keysFound = GetKeysForEncryptedUtfTable(magic, &seed, &increment);
            if (!keysFound) {
                return FALSE;
            } else {
                _utfReader = new CUtfReader(seed, increment);
            }
        }
        return TRUE;
    }

    bool_t CUtfTable::GetKeysForEncryptedUtfTable(const uint8_t *magic, uint8_t *seed, uint8_t *incr) {
        for (auto s = 0; s <= 0xff; ++s) {
            if ((magic[0] ^ s) != UTF_SIGNATURE[0]) {
                continue;
            }
            for (auto i = 0; i <= 0xff; ++i) {
                auto m = s * i;
                if ((magic[1] ^ m) != UTF_SIGNATURE[1]) {
                    continue;
                }
                auto t = i;
                for (auto j = 2; j < 4; ++j) {
                    m *= t;
                    if ((magic[j] ^ m) != UTF_SIGNATURE[j]) {
                        break;
                    }
                    if (j < 3) {
                        continue;
                    }
                    if (seed) {
                        *seed = static_cast<uint8_t>(s);
                    }
                    if (incr) {
                        *incr = static_cast<uint8_t>(i);
                    }
                    return TRUE;
                }
            }
        }
        return FALSE;
    }

    CMemoryStream *CUtfTable::GetTableDataStream() {
        auto *stream = _stream;
        const auto streamOffset = _streamOffset;
        auto *reader = _utfReader;
        const auto tableSize = reader->PeekUInt32(stream, streamOffset, 4) + 8;

        if (!IsEncrypted()) {
            return CAcbHelper::ExtractToNewStream(stream, streamOffset, tableSize);
        }

        auto originalPosition = stream->GetPosition();
        uint32_t totalBytesRead = 0;

        auto *memoryStream = new CMemoryStream(tableSize, FALSE);
        uint32_t currentIndex = 0;
        auto currentOffset = streamOffset;
        uint8_t *buffer;

        do {
            auto shouldRead = tableSize - totalBytesRead;
            buffer = static_cast<uint8_t *>(malloc(shouldRead));

            _utfReader->PeekBytes(stream, buffer, 0, currentOffset, shouldRead, totalBytesRead);
            memoryStream->Write(buffer, shouldRead, 0, shouldRead);

            // TODO: Actually this should be "+= buffer.length", but now CUtfReader::PeekBytes() does not return number of bytes read.
            currentOffset += shouldRead;
            currentIndex += shouldRead;
            totalBytesRead += shouldRead;

            free(buffer);
        } while (totalBytesRead < tableSize);

        stream->SetPosition(originalPosition);

        memoryStream->Seek(0, StreamSeekOrigin::Begin);

        return memoryStream;
    }

    void CUtfTable::ReadUtfHeader(IStream *stream, UTF_HEADER &header, char *tableNameBuffer) {
        ReadUtfHeader(stream, stream->GetPosition(), header, tableNameBuffer);
    }

    void CUtfTable::ReadUtfHeader(IStream *stream, uint64_t streamOffset, UTF_HEADER &header, char *tableNameBuffer) {
        CBinaryReader reader(stream);
        auto pos = stream->GetPosition();

        reader.Seek(streamOffset + 4, StreamSeekOrigin::Begin);

        header.tableSize = reader.ReadUInt32BE();
        header.unk1 = reader.ReadUInt16BE();
        header.perRowDataOffset = (uint32_t)reader.ReadUInt16BE() + 8;
        header.stringTableOffset = reader.ReadUInt32BE() + 8;
        header.extraDataOffset = reader.ReadUInt32BE() + 8;
        header.tableNameOffset = reader.ReadUInt32BE();
        header.fieldCount = reader.ReadUInt16BE();
        header.rowSize = reader.ReadUInt16BE();
        header.rowCount = reader.ReadUInt32BE();

        if (tableNameBuffer) {
            stream->Seek(header.stringTableOffset + header.tableNameOffset, StreamSeekOrigin::Begin);
            CStreamExtensions::ReadNullEndedString(stream, tableNameBuffer, UTF_TABLE_MAX_NAME_LEN);
        }

        stream->Seek(pos, StreamSeekOrigin::Begin);
    }

    void CUtfTable::InitializeUtfSchema(IStream *sourceStream, CMemoryStream *tableDataStream, uint64_t schemaOffset) {
        const auto &header = _utfHeader;
        const auto baseOffset = _streamOffset;
        auto &rows = _rows;
        char fieldNameBuffer[UTF_FIELD_MAX_NAME_LEN] = {'\0'};

        for (auto i = 0; i < header.rowCount; ++i) {
            auto currentStreamOffset = schemaOffset;
            UtfRow row;
            uint32_t currentRowOffset = 0;
            row.baseOffset = header.perRowDataOffset + header.rowSize * i;

            for (auto j = 0; j < header.fieldCount; ++j) {
                auto field = new CUtfField();
                const auto columnType = CBinaryReader::PeekUInt8(tableDataStream, currentStreamOffset);
                const auto nameOffset = CBinaryReader::PeekInt32BE(tableDataStream, currentStreamOffset + 1);
                const auto pos = tableDataStream->GetPosition();
                tableDataStream->Seek(header.stringTableOffset + nameOffset, StreamSeekOrigin::Begin);
                CStreamExtensions::ReadNullEndedString(tableDataStream, fieldNameBuffer, sizeof(fieldNameBuffer));
                field->SetName(fieldNameBuffer);
                tableDataStream->Seek(pos, StreamSeekOrigin::Begin);

                const auto storage = static_cast<UtfColumnStorage>(columnType & CGSS_UTF_COLUMN_STORAGE_MASK);
                const auto type = static_cast<UtfColumnType>(columnType & CGSS_UTF_COLUMN_TYPE_MASK);

                field->type = static_cast<CGSS_UTF_COLUMN_TYPE>(type);
                field->storage = static_cast<CGSS_UTF_COLUMN_STORAGE>(storage);

                switch (storage) {
                    case UtfColumnStorage::Const:
                    case UtfColumnStorage::Const2: {
                        const auto constantOffset = static_cast<uint32_t>(currentStreamOffset) + 5;
                        field->offsetInRow = 0; // constant

                        switch (type) {
                            case UtfColumnType::U8:
                                field->SetValue(CBinaryReader::PeekUInt8(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 1;
                                break;
                            case UtfColumnType::S8:
                                field->SetValue(CBinaryReader::PeekInt8(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 1;
                                break;
                            case UtfColumnType::U16:
                                field->SetValue(CBinaryReader::PeekUInt16BE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 2;
                                break;
                            case UtfColumnType::S16:
                                field->SetValue(CBinaryReader::PeekInt16BE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 2;
                                break;
                            case UtfColumnType::U32:
                                field->SetValue(CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 4;
                                break;
                            case UtfColumnType::S32:
                                field->SetValue(CBinaryReader::PeekInt32BE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 4;
                                break;
                            case UtfColumnType::U64:
                                field->SetValue(CBinaryReader::PeekUInt64BE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 8;
                                break;
                            case UtfColumnType::S64:
                                field->SetValue(CBinaryReader::PeekInt64BE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 8;
                                break;
                            case UtfColumnType::R32:
                                field->SetValue(CBinaryReader::PeekSingleBE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 4;
                                break;
                            case UtfColumnType::R64:
                                field->SetValue(CBinaryReader::PeekDoubleBE(tableDataStream, constantOffset), constantOffset);
                                currentStreamOffset += 8;
                                break;
                            case UtfColumnType::String: {
                                const auto dataOffset = CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset);
                                const auto fieldDataOffset = header.stringTableOffset + dataOffset;
                                const auto *str = reinterpret_cast<const char *>(tableDataStream->GetBuffer()) + fieldDataOffset;
                                field->SetValue(str, dataOffset);
                                currentStreamOffset += 4;
                                break;
                            }
                            case UtfColumnType::Data: {
                                const auto dataOffset = CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset);
                                const auto dataSize = CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset + 4);
                                const auto fieldDataOffset = static_cast<uint32_t>(baseOffset) + header.extraDataOffset + dataOffset;
                                if (dataSize > 0) {
                                    auto dataBuffer = static_cast<uint8_t *>(malloc(dataSize));
                                    memset(dataBuffer, 0, dataSize);
                                    sourceStream->Seek(fieldDataOffset, StreamSeekOrigin::Begin);
                                    sourceStream->Read(dataBuffer, dataSize, 0, dataSize);
                                    field->SetValue(dataBuffer, dataSize, fieldDataOffset);
                                    free(dataBuffer);
                                } else {
                                    field->SetValue(nullptr, dataSize, fieldDataOffset);
                                }
                                currentStreamOffset += 8;
                                break;
                            }
                            default:
                                throw CFormatException("Unknown UTF table field type.");
                        }
                        break;
                    }
                    case UtfColumnStorage::PerRow: {
                        auto fieldDataOffset = row.baseOffset + currentRowOffset;
                        field->offsetInRow = currentRowOffset;

                        switch (type) {
                            case UtfColumnType::U8:
                                field->SetValue(CBinaryReader::PeekUInt8(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 1;
                                break;
                            case UtfColumnType::S8:
                                field->SetValue(CBinaryReader::PeekInt8(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 1;
                                break;
                            case UtfColumnType::U16:
                                field->SetValue(CBinaryReader::PeekUInt16BE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 2;
                                break;
                            case UtfColumnType::S16:
                                field->SetValue(CBinaryReader::PeekInt16BE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 2;
                                break;
                            case UtfColumnType::U32:
                                field->SetValue(CBinaryReader::PeekUInt32BE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 4;
                                break;
                            case UtfColumnType::S32:
                                field->SetValue(CBinaryReader::PeekInt32BE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 4;
                                break;
                            case UtfColumnType::U64:
                                field->SetValue(CBinaryReader::PeekInt64BE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 8;
                                break;
                            case UtfColumnType::S64:
                                field->SetValue(CBinaryReader::PeekInt64BE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 8;
                                break;
                            case UtfColumnType::R32:
                                field->SetValue(CBinaryReader::PeekSingleBE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 4;
                                break;
                            case UtfColumnType::R64:
                                field->SetValue(CBinaryReader::PeekDoubleBE(tableDataStream, fieldDataOffset), fieldDataOffset);
                                currentRowOffset += 8;
                                break;
                            case UtfColumnType::String: {
                                const auto rowDataOffset = CBinaryReader::PeekUInt32BE(tableDataStream, row.baseOffset + currentRowOffset);
                                fieldDataOffset = header.stringTableOffset + rowDataOffset;
                                const auto *str = reinterpret_cast<const char *>(tableDataStream->GetBuffer()) + fieldDataOffset;
                                field->SetValue(str, fieldDataOffset);
                                currentRowOffset += 4;
                                break;
                            }
                            case UtfColumnType::Data: {
                                const auto rowDataOffset = CBinaryReader::PeekUInt32BE(tableDataStream, row.baseOffset + currentRowOffset);
                                const auto dataSize = CBinaryReader::PeekUInt32BE(tableDataStream, row.baseOffset + currentRowOffset + 4);
                                fieldDataOffset = static_cast<uint32_t>(baseOffset) + header.extraDataOffset + rowDataOffset;
                                if (dataSize > 0) {
                                    auto dataBuffer = static_cast<uint8_t *>(malloc(dataSize));
                                    memset(dataBuffer, 0, dataSize);
                                    sourceStream->Seek(fieldDataOffset, StreamSeekOrigin::Begin);
                                    sourceStream->Read(dataBuffer, dataSize, 0, dataSize);
                                    field->SetValue(dataBuffer, dataSize, fieldDataOffset);
                                    free(dataBuffer);
                                } else {
                                    field->SetValue(nullptr, dataSize, fieldDataOffset);
                                }
                                currentRowOffset += 8;
                                break;
                            }
                            default:
                                throw CFormatException("Unknown UTF table field type.");
                        }
                        break;
                    }
                    default:
                        throw CFormatException("Unknown UTF table field storage format.");
                }

                row.fields.push_back(field);

                currentStreamOffset += 5;
            }

            rows.push_back(row);
        }
    }

    bool_t CUtfTable::GetFieldOffset(uint32_t rowIndex, const char *fieldName, uint64_t *offset) {
        if (rowIndex >= _rows.size()) {
            if (offset) {
                *offset = 0;
            }

            return FALSE;
        }

        const auto &row = _rows[rowIndex];

        for (auto &field : row.fields) {
            if (strcmp(fieldName, field->name) == 0) {
                if (offset) {
                    *offset = field->offset;
                }

                return TRUE;
            }
        }

        return FALSE;
    }

    bool_t CUtfTable::GetFieldSize(uint32_t rowIndex, const char *fieldName, uint32_t *size) {
        if (rowIndex >= _rows.size()) {
            if (size) {
                *size = 0;
            }

            return FALSE;
        }

        const auto &row = _rows[rowIndex];

        for (auto &field : row.fields) {
            if (strcmp(fieldName, field->name) == 0) {
                if (size) {
                    *size = field->value.data.size;
                }

                return TRUE;
            }
        }

        return FALSE;
    }

CGSS_NS_END
