#include "../cgss_api.h"
#include "../cgss_enum.h"
#include "CHandleManager.h"

using namespace cgss;

using HandleType = CHandleManager::HandleType;

DEFINE_ENUM_CLS_BINARY_OP(HandleType, |);

DEFINE_ENUM_CLS_BINARY_OP(HandleType, &);

DEFINE_ENUM_CLS_UNARY_OP(HandleType, !);

static inline IStream *to_stream(uint32_t handle) {
    return CHandleManager::getInstance()->getHandlePtr(handle);
}

// CLion crashes if this macro is wrapped with do...while(0).
#define CHECK_HANDLE(handle) \
    if (!CHandleManager::getInstance()->handleExists(handle)) { \
        return CGSS_OP_HANDLE_INVALID; \
    }

CGSS_API_IMPL(void) cgssTest() {
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

inline void alloc_stream(CGSS_HANDLE *handle, IStream *stream, HandleType type) {
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

CGSS_API_IMPL(void) cgssWaveDecode8BitU(float data, uint8_t *buffer, uint32_t *cursor) {
    CDefaultWaveGenerator::Decode8BitU(data, buffer, cursor);
}

CGSS_API_IMPL(void) cgssWaveDecode16BitS(float data, uint8_t *buffer, uint32_t *cursor) {
    CDefaultWaveGenerator::Decode16BitS(data, buffer, cursor);
}

CGSS_API_IMPL(void) cgssWaveDecode24BitS(float data, uint8_t *buffer, uint32_t *cursor) {
    CDefaultWaveGenerator::Decode24BitS(data, buffer, cursor);
}

CGSS_API_IMPL(void) cgssWaveDecode32BitS(float data, uint8_t *buffer, uint32_t *cursor) {
    CDefaultWaveGenerator::Decode32BitS(data, buffer, cursor);
}

CGSS_API_IMPL(void) cgssWaveDecodeFloat(float data, uint8_t *buffer, uint32_t *cursor) {
    CDefaultWaveGenerator::DecodeFloat(data, buffer, cursor);
}
