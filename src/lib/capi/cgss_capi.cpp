#include <vector>
#include <algorithm>
#include "../cgss_api.h"
#include "CHandleManager.h"
#include "../cdata/UTF_FIELD.h"

using namespace cgss;

using HandleType = CHandleManager::HandleType;

DEFINE_ENUM_CLS_BINARY_OP(HandleType, |);

DEFINE_ENUM_CLS_BINARY_OP(HandleType, &);

DEFINE_ENUM_CLS_UNARY_OP(HandleType, !);

std::string g_lastErrorString;

static IStream *to_stream(uint32_t handle) {
    return CHandleManager::getInstance()->getHandlePtr(handle);
}

static void cgssSetLastErrorMessage(const std::string &str) {
    g_lastErrorString = str;
}

// CLion crashes if this macro is wrapped with do...while(0).
#define CHECK_HANDLE(handle) \
    if (!CHandleManager::getInstance()->handleExists(handle)) { \
        return CGSS_OP_INVALID_HANDLE; \
    }

CGSS_API_IMPL(void) cgssTest() {
}

CGSS_API_IMPL(void) cgssGetOpResultString(CGSS_OP_RESULT error, char *buffer, size_t length) {
    if (!buffer) {
        return;
    }

#define PRINT_ERR_STR(str) snprintf(buffer, length, str)

    switch (error) {
        case CGSS_OP_OK:
            PRINT_ERR_STR("No error");
            break;
        case CGSS_OP_GENERIC_FAULT:
            PRINT_ERR_STR("Generic fault");
            break;
        case CGSS_OP_BUFFER_TOO_SMALL:
            PRINT_ERR_STR("Buffer too small");
            break;
        case CGSS_OP_NOT_IMPLEMENTED:
            PRINT_ERR_STR("Not implemented");
            break;
        case CGSS_OP_INVALID_OPERATION:
            PRINT_ERR_STR("Invalid operation");
            break;
        case CGSS_OP_INVALID_ARGUMENT:
            PRINT_ERR_STR("Invalid argument");
            break;
        case CGSS_OP_FORMAT_ERROR:
            PRINT_ERR_STR("Format error");
            break;
        case CGSS_OP_CHECKSUM_ERROR:
            PRINT_ERR_STR("Checksum error");
            break;
        case CGSS_OP_DECODE_FAILED:
            PRINT_ERR_STR("Decode failed");
            break;
        case CGSS_OP_INVALID_HANDLE:
            PRINT_ERR_STR("Invalid handle");
            break;
        default:
            break;
    }

#undef PRINT_ERR_STR
}

CGSS_API_IMPL(const char *)cgssGetLastErrorMessage() {
    return g_lastErrorString.c_str();
}

CGSS_API_IMPL(bool_t) cgssHelperFileExists(LPCSTR fileName) {
    if (!fileName) {
        return FALSE;
    }

    return CFileSystem::FileExists(fileName);
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamRead(CGSS_HANDLE handle, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *read) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->Read(buffer, bufferSize, offset, count);
        if (read) {
            *read = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamWrite(CGSS_HANDLE handle, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *written) {
    CHECK_HANDLE(handle);
    try {
        const auto w = to_stream(handle)->Write(buffer, bufferSize, offset, count);
        if (written) {
            *written = w;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamSeek(CGSS_HANDLE handle, int64_t offset, CGSS_STREAM_SEEK_ORIGIN origin) {
    CHECK_HANDLE(handle);
    try {
        to_stream(handle)->Seek(offset, static_cast<StreamSeekOrigin>(origin));
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamIsReadable(CGSS_HANDLE handle, _OUT_ bool_t *isReadable) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->IsReadable();
        if (isReadable) {
            *isReadable = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamIsWritable(CGSS_HANDLE handle, _OUT_ bool_t *isWritable) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->IsWritable();
        if (isWritable) {
            *isWritable = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamIsSeekable(CGSS_HANDLE handle, _OUT_ bool_t *isSeekable) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->IsSeekable();
        if (isSeekable) {
            *isSeekable = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamGetPosition(CGSS_HANDLE handle, _OUT_ uint64_t *position) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->GetPosition();
        if (position) {
            *position = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamSetPosition(CGSS_HANDLE handle, uint64_t position) {
    CHECK_HANDLE(handle);
    try {
        to_stream(handle)->SetPosition(position);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamGetLength(CGSS_HANDLE handle, _OUT_ uint64_t *length) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->GetLength();
        if (length) {
            *length = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamSetLength(CGSS_HANDLE handle, uint64_t length) {
    CHECK_HANDLE(handle);
    try {
        to_stream(handle)->SetLength(length);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamReadByte(CGSS_HANDLE handle, _OUT_ uint8_t *byte) {
    CHECK_HANDLE(handle);
    try {
        const auto r = to_stream(handle)->ReadByte();
        if (byte) {
            *byte = r;
        }
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamWriteByte(CGSS_HANDLE handle, uint8_t byte) {
    CHECK_HANDLE(handle);
    try {
        to_stream(handle)->WriteByte(byte);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamFlush(CGSS_HANDLE handle) {
    CHECK_HANDLE(handle);
    try {
        to_stream(handle)->Flush();
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamCopyTo(CGSS_HANDLE source, CGSS_HANDLE destination) {
    CHECK_HANDLE(source);
    CHECK_HANDLE(destination);
    try {
        to_stream(source)->CopyTo(*to_stream(destination));
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamCopyTo2(CGSS_HANDLE source, CGSS_HANDLE destination, uint32_t bufferSize) {
    CHECK_HANDLE(source);
    CHECK_HANDLE(destination);
    try {
        to_stream(source)->CopyTo(*to_stream(destination), bufferSize);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCloseHandle(CGSS_HANDLE handle) {
    CHECK_HANDLE(handle);
    try {
        CHandleManager::getInstance()->free(handle, TRUE);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

static void alloc_stream(CGSS_HANDLE *handle, IStream *stream, HandleType type) {
    CGSS_HANDLE h = CHandleManager::getInstance()->alloc(stream, type);
    *handle = h;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateFileStream(LPCSTR fileName, _OUT_ CGSS_HANDLE *stream) {
    if (!stream) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    alloc_stream(stream, new CFileStream(fileName), HandleType::CStream);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateFileStream2(LPCSTR fileName, CGSS_FILE_MODE fileMode, _OUT_ CGSS_HANDLE *stream) {
    if (!stream) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    alloc_stream(stream, new CFileStream(fileName, static_cast<FileMode>(fileMode)), HandleType::CStream);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateFileStream3(LPCSTR fileName, CGSS_FILE_MODE fileMode, CGSS_FILE_ACCESS fileAccess, _OUT_ CGSS_HANDLE *stream) {
    if (!stream) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    alloc_stream(stream, new CFileStream(fileName, static_cast<FileMode>(fileMode), static_cast<FileAccess>(fileAccess)), HandleType::CStream);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateHcaDecoder(CGSS_HANDLE baseStream, _OUT_ CGSS_HANDLE *decoder) {
    CHECK_HANDLE(baseStream);
    if (!decoder) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    alloc_stream(decoder, new CHcaDecoder(to_stream(baseStream)), HandleType::CStream | HandleType::CHcaReaderBase);
    return CGSS_OP_OK;
}
CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateHcaDecoder2(CGSS_HANDLE baseStream, const HCA_DECODER_CONFIG *decoderConfig, _OUT_ CGSS_HANDLE *decoder) {
    CHECK_HANDLE(baseStream);
    if (!decoderConfig || !decoder) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    alloc_stream(decoder, new CHcaDecoder(to_stream(baseStream), *decoderConfig), HandleType::CStream | HandleType::CHcaReaderBase);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateCipherConverter(CGSS_HANDLE baseStream, const HCA_CIPHER_CONFIG *cryptFrom, const HCA_CIPHER_CONFIG *cryptTo,
                                                        _OUT_ CGSS_HANDLE *converter) {
    CHECK_HANDLE(baseStream);
    if (!cryptFrom || !cryptTo || !converter) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    alloc_stream(converter, new CHcaCipherConverter(to_stream(baseStream), *cryptFrom, *cryptTo), HandleType::CStream | HandleType::CHcaReaderBase);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssGetHcaInfo(CGSS_HANDLE handle, HCA_INFO *info) {
    CHECK_HANDLE(handle);
    if (!info) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    HandleType handleType = CHandleManager::getInstance()->getHandleType(handle);
    if ((handleType & HandleType::CHcaReaderBase) == HandleType::None) {
        return CGSS_OP_INVALID_OPERATION;
    }
    auto *reader = dynamic_cast<CHcaFormatReader *>(CHandleManager::getInstance()->getHandlePtr(handle));
    HCA_INFO &i = *info;
    reader->GetHcaInfo(i);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(uint32_t) cgssWaveDecode8BitU(float data, uint8_t *buffer, uint32_t cursor) {
    return CDefaultWaveGenerator::Decode8BitU(data, buffer, cursor);
}

CGSS_API_IMPL(uint32_t) cgssWaveDecode16BitS(float data, uint8_t *buffer, uint32_t cursor) {
    return CDefaultWaveGenerator::Decode16BitS(data, buffer, cursor);
}

CGSS_API_IMPL(uint32_t) cgssWaveDecode24BitS(float data, uint8_t *buffer, uint32_t cursor) {
    return CDefaultWaveGenerator::Decode24BitS(data, buffer, cursor);
}

CGSS_API_IMPL(uint32_t) cgssWaveDecode32BitS(float data, uint8_t *buffer, uint32_t cursor) {
    return CDefaultWaveGenerator::Decode32BitS(data, buffer, cursor);
}

CGSS_API_IMPL(uint32_t) cgssWaveDecodeFloat(float data, uint8_t *buffer, uint32_t cursor) {
    return CDefaultWaveGenerator::DecodeFloat(data, buffer, cursor);
}

static void copy_utf_field(UTF_FIELD *dest, const UTF_FIELD *src) {
    memcpy(dest, src, sizeof(UTF_FIELD));
    memset(dest->name, 0, sizeof(dest->name));
    strncpy(dest->name, src->name, UTF_FIELD_MAX_NAME_LEN);

    switch (dest->type) {
        case CGSS_UTF_COLUMN_TYPE_DATA:
            if (src->value.data.ptr && src->value.data.size > 0) {
                dest->value.data.ptr = malloc(src->value.data.size);
                memcpy(dest->value.data.ptr, src->value.data.ptr, src->value.data.size);
            }
            break;
        case CGSS_UTF_COLUMN_TYPE_STRING: {
            const auto strLength = src->value.str ? strlen(src->value.str) : 0;
            if (strLength > 0) {
                dest->value.str = (char *)malloc(strLength + 1);
                strncpy(dest->value.str, src->value.str, strLength);
                dest->value.str[strLength] = '\0';
            }
            break;
        }
        default:
            break;
    }
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssUtfReadTable(CGSS_HANDLE stream, uint64_t offset, UTF_TABLE **table) {
    CHECK_HANDLE(stream);

    if (!table) {
        return CGSS_OP_INVALID_ARGUMENT;
    }

    *table = nullptr;

    auto s = to_stream(stream);

    CUtfTable *tableCppObject = nullptr;

    try {
        tableCppObject = new CUtfTable(s, offset);
    } catch (CFormatException &ex) {
        cgssSetLastErrorMessage(ex.GetExceptionMessage());
        return CGSS_OP_FORMAT_ERROR;
    } catch (CException &ex) {
        cgssSetLastErrorMessage(ex.GetExceptionMessage());
        return CGSS_OP_GENERIC_FAULT;
    }

    auto utfTable = new UTF_TABLE();
    // So we don't expose a constructor to UTF_SCHEMA struct.
    memset(utfTable, 0, sizeof(UTF_TABLE));

    tableCppObject->GetHeader(utfTable->header);

    strncpy(utfTable->tableName, tableCppObject->GetName(), UTF_TABLE_MAX_NAME_LEN);

    const auto &rows = tableCppObject->GetRows();
    utfTable->rows = new UTF_ROW[utfTable->header.rowCount];

    std::vector<UTF_FIELD> fieldList;

    uint32_t i = 0;
    for (auto &row : rows) {
        auto &utfRow = utfTable->rows[i];
        fieldList.clear();

        if (utfTable->header.fieldCount > 0) {
            utfRow.fields = new UTF_FIELD[utfTable->header.fieldCount];
        } else {
            utfRow.fields = nullptr;
        }

        auto j = 0;
        for (const auto pField : row.fields) {
            copy_utf_field(utfRow.fields + j, pField);
            ++j;
        }

        /*
        for (j = 0; j < utfTable->header.fieldCount; ++j) {
            fieldList.push_back(utfRow.fields[j]);
        }

        std::sort(fieldList.begin(), fieldList.end(), [](const UTF_FIELD &left, const UTF_FIELD &right) {
            if (left.offsetInRow < right.offsetInRow) {
                return true;
            } else if (left.offsetInRow > right.offsetInRow) {
                return false;
            } else {
                return left.offset < right.offset;
            }
        });

        for (j = 0; j < utfTable->header.fieldCount; ++j) {
            utfRow.fields[j] = fieldList[j];
        }
         */

        ++i;
    }

    delete tableCppObject;

    *table = utfTable;

    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssUtfFreeTable(UTF_TABLE *table) {
    if (!table) {
        return CGSS_OP_INVALID_ARGUMENT;
    }

    for (auto i = 0; i < table->header.rowCount; ++i) {
        auto &row = table->rows[i];

        for (auto j = 0; j < table->header.fieldCount; ++j) {
            auto &field = row.fields[j];

            switch (field.type) {
                case CGSS_UTF_COLUMN_TYPE_STRING:
                    if (field.value.str) {
                        free(field.value.str);
                        field.value.str = nullptr;
                    }
                    break;
                case CGSS_UTF_COLUMN_TYPE_DATA:
                    if (field.value.data.ptr) {
                        free(field.value.data.ptr);
                        field.value.data.ptr = nullptr;
                    }
                    field.value.data.size = 0;
                    break;
                default:
                    break;
            }
        }

        delete row.fields;
    }

    delete (table->rows);

    memset(table, 0, sizeof(UTF_TABLE));

    delete table;

    return CGSS_OP_OK;
}

CGSS_API_IMPL(bool_t) cgssUtfTryParseTable(void *data, size_t dataSize, UTF_TABLE **table) {
    if (!data || dataSize <= 0) {
        return FALSE;
    }

    CGSS_HANDLE handle;
    CMemoryStream memory(static_cast<uint8_t *>(data), dataSize, FALSE);
    alloc_stream(&handle, &memory, HandleType::CStream);

    const auto r = cgssUtfReadTable(handle, 0, table);

    cgssCloseHandle(handle);

    return CGSS_OP_SUCCEEDED(r) ? 1 : 0;
}
