#include "../cgss_api.h"

using namespace cgss;

#define to_stream(ptr) (reinterpret_cast<IStream *>(ptr))
#define to_pstream(ptr) (reinterpret_cast<IStream **>(ptr))

CGSS_API_IMPL(void) cgssTest() {
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamRead(CGSS_HSTREAM stream, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *read) {
    try {
        const auto r = to_stream(stream)->Read(buffer, bufferSize, offset, count);
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamWrite(CGSS_HSTREAM stream, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *written) {
    try {
        const auto w = to_stream(stream)->Write(buffer, bufferSize, offset, count);
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamSeek(CGSS_HSTREAM stream, int64_t offset, CGSS_STREAM_SEEK_ORIGIN origin) {
    try {
        to_stream(stream)->Seek(offset, static_cast<StreamSeekOrigin>(origin));
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamIsReadable(CGSS_HSTREAM stream, _OUT_ bool_t *isReadable) {
    try {
        const auto r = to_stream(stream)->IsReadable();
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamIsWritable(CGSS_HSTREAM stream, _OUT_ bool_t *isWritable) {
    try {
        const auto r = to_stream(stream)->IsWritable();
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamIsSeekable(CGSS_HSTREAM stream, _OUT_ bool_t *isSeekable) {
    try {
        const auto r = to_stream(stream)->IsSeekable();
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamGetPosition(CGSS_HSTREAM stream, _OUT_ uint64_t *position) {
    try {
        const auto r = to_stream(stream)->GetPosition();
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamSetPosition(CGSS_HSTREAM stream, uint64_t position) {
    try {
        to_stream(stream)->SetPosition(position);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamGetLength(CGSS_HSTREAM stream, _OUT_ uint64_t *length) {
    try {
        const auto r = to_stream(stream)->GetLength();
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamSetLength(CGSS_HSTREAM stream, uint64_t length) {
    try {
        to_stream(stream)->SetLength(length);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamReadByte(CGSS_HSTREAM stream, _OUT_ uint8_t *byte) {
    try {
        const auto r = to_stream(stream)->ReadByte();
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

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamWriteByte(CGSS_HSTREAM stream, uint8_t byte) {
    try {
        to_stream(stream)->WriteByte(byte);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamFlush(CGSS_HSTREAM stream) {
    try {
        to_stream(stream)->Flush();
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamCopyTo(CGSS_HSTREAM source, CGSS_HSTREAM destination) {
    try {
        to_stream(source)->CopyTo(*to_stream(destination));
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssStreamCopyTo2(CGSS_HSTREAM source, CGSS_HSTREAM destination, uint32_t bufferSize) {
    try {
        to_stream(source)->CopyTo(*to_stream(destination), bufferSize);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCloseStream(CGSS_HSTREAM stream) {
    try {
        delete to_stream(stream);
    } catch (const CException &ex) {
        return ex.GetOpResult();
    } catch (...) {
        return CGSS_OP_GENERIC_FAULT;
    }
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateFileStream(LPCSTR fileName, _OUT_ CGSS_HSTREAM **stream) {
    if (!stream) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    *to_pstream(stream) = new CFileStream(fileName);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateFileStream2(LPCSTR fileName, CGSS_FILE_MODE fileMode, _OUT_ CGSS_HSTREAM **stream) {
    if (!stream) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    *to_pstream(stream) = new CFileStream(fileName, static_cast<FileMode>(fileMode));
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateFileStream3(LPCSTR fileName, CGSS_FILE_MODE fileMode, CGSS_FILE_ACCESS fileAccess, _OUT_ CGSS_HSTREAM **stream) {
    if (!stream) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    *to_pstream(stream) = new CFileStream(fileName, static_cast<FileMode>(fileMode), static_cast<FileAccess>(fileAccess));
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateHcaDecoder(CGSS_HSTREAM baseStream, _OUT_ CGSS_HSTREAM **decoder) {
    if (!baseStream || !decoder) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    *to_pstream(decoder) = new CHcaDecoder(to_stream(baseStream));
    return CGSS_OP_OK;
}
CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateHcaDecoder2(CGSS_HSTREAM baseStream, const HCA_DECODER_CONFIG *decoderConfig, _OUT_ CGSS_HSTREAM **decoder) {
    if (!baseStream || !decoderConfig || !decoder) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    *to_pstream(decoder) = new CHcaDecoder(to_stream(baseStream), *decoderConfig);
    return CGSS_OP_OK;
}

CGSS_API_IMPL(CGSS_OP_RESULT) cgssCreateCipherConverter(CGSS_HSTREAM baseStream, const HCA_CIPHER_CONFIG *cryptFrom, const HCA_CIPHER_CONFIG *cryptTo,
                                                        _OUT_ CGSS_HSTREAM **converter) {
    if (!baseStream || !cryptFrom || !cryptTo) {
        return CGSS_OP_INVALID_ARGUMENT;
    }
    *to_pstream(converter) = new CHcaCipherConverter(to_stream(baseStream), *cryptFrom, *cryptTo);
    return CGSS_OP_OK;
}
