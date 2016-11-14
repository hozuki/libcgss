#include <stdlib.h>
#include <stdexcept>
#include <stdio.h>
#include "tm_file_stream.h"

#ifndef __MINGW_H
#include <algorithm>
#endif

using namespace std;

#define __FILE_READ ("rb")
#define __FILE_WRITE ("wb")
#define __FILE_APPEND ("ab")
#define __FILE_READ_U ("rb+")
#define __FILE_WRITE_U ("wb+")

FileStream::FileStream(LPCSTR fileName)
        : FileStream(fileName, MODE_OPEN_OR_CREATE) {
}

FileStream::FileStream(LPCSTR fileName, FileMode mode)
        : FileStream(fileName, mode, ACCESS_READ_WRITE) {
}

FileStream::FileStream(LPCSTR fileName, FileMode mode, FileAccess access) {
    _mode = mode;
    _access = access;
    _fp = OpenFile(fileName);
}

FileStream::~FileStream() {
    if (_fp) {
        fclose(_fp);
    }
    _fp = nullptr;
}

uint32 FileStream::Read(uint8 *buffer, uint32 bufferSize, size_t offset, uint32 count) {
    if (!buffer) {
        throw invalid_argument("FileStream::Read()");
    }
    if (!IsReadable()) {
        throw runtime_error("FileStream::Read()");
    }
    auto actualCount = min(bufferSize - offset, count);
    auto actualRead = fread(buffer + offset, actualCount, 1, _fp);
    return actualRead;
}

uint32 FileStream::Write(const uint8 *buffer, uint32 bufferSize, uint32 offset, uint32 count) {
    if (!buffer) {
        throw invalid_argument("FileStream::Write()");
    }
    if (!IsWritable()) {
        throw runtime_error("FileStream::Write()");
    }
    auto actualCount = min(bufferSize - offset, count);
    fwrite(buffer + offset, actualCount, 1, _fp);
    return actualCount;
}

ubool FileStream::IsWritable() const {
    return _isWritable;
}

ubool FileStream::IsReadable() const {
    return _isReadable;
}

ubool FileStream::IsSeekable() const {
    return _isSeekable;
}

uint64 FileStream::GetPosition() const {
    return (uint64)ftell(_fp);
}

void FileStream::SetPosition(uint64 value) {
    fseek(_fp, (long)value, SEEK_SET);
}

uint64 FileStream::GetLength() const {
    auto position = ftell(_fp);
    fseek(_fp, 0, SEEK_END);
    auto r = ftell(_fp);
    fseek(_fp, position, SEEK_SET);
    return (uint64)r;
}

void FileStream::SetLength(uint64 value) {
    fseek(_fp, (long)value, SEEK_SET);
}

void FileStream::Flush() {
    fflush(_fp);
}

ubool FileStream::FileExists(LPCSTR fileName) {
    auto fp = fopen(fileName, "r");
    if (!fp) {
        return FALSE;
    }
    fclose(fp);
    return TRUE;
}

FILE *FileStream::OpenFile(LPCSTR fileName) {
#define __OUT() throw out_of_range("Mode/Access: out of range")
#define __CMB() throw runtime_error("Mode/Access: incompatible")
#define __EXT() throw runtime_error("File exists: " + std::string(fileName))
#define __NEX() throw runtime_error("File doesn't exist: " + std::string(fileName))
    auto m = _mode;
    auto a = _access;
    _isReadable = (ubool)(a & ACCESS_READ);
    _isWritable = (ubool)(a & ACCESS_WRITE);
    _isSeekable = (ubool)(m != MODE_APPEND);
    if (m == MODE_NONE || a == ACCESS_NONE) {
        __OUT();
    }
    LPCSTR fileMode = nullptr;
    switch (m) {
        case MODE_APPEND:
            switch (a) {
                case ACCESS_WRITE:
                    fileMode = __FILE_APPEND;
                    break;
                case ACCESS_READ:
                case ACCESS_READ_WRITE:
                    __CMB();
                default:
                    __OUT();
            }
            break;
        case MODE_CREATE:
            switch (a) {
                case ACCESS_WRITE:
                    fileMode = __FILE_WRITE_U;
                    break;
                case ACCESS_READ:
                    CreateFile(fileName);
                    fileMode = __FILE_READ;
                    break;
                case ACCESS_READ_WRITE:
                    CreateFile(fileName);
                    fileMode = __FILE_READ_U;
                    break;
                default:
                    __OUT();
            }
            break;
        case MODE_CREATE_NEW:
            if (FileExists(fileName)) {
                __EXT();
            }
            switch (a) {
                case ACCESS_WRITE:
                case ACCESS_READ:
                case ACCESS_READ_WRITE:
                    fileMode = __FILE_WRITE_U;
                    break;
                default:
                    __OUT();
            }
            break;
        case MODE_OPEN_EXISTING:
            if (!FileExists(fileName)) {
                __NEX();
            }
            switch (a) {
                case ACCESS_WRITE:
                    fileMode = __FILE_WRITE;
                    break;
                case ACCESS_READ:
                    fileMode = __FILE_READ;
                    break;
                case ACCESS_READ_WRITE:
                    fileMode = __FILE_READ_U;
                    break;
                default:
                    __OUT();
            }
            break;
        case MODE_OPEN_OR_CREATE:
            if (!FileExists(fileName)) {
                CreateFile(fileName);
            }
            switch (a) {
                case ACCESS_WRITE:
                    fileMode = __FILE_WRITE;
                    break;
                case ACCESS_READ:
                    fileMode = __FILE_READ;
                    break;
                case ACCESS_READ_WRITE:
                    fileMode = __FILE_READ_U;
                    break;
                default:
                    __OUT();
            }
            break;
        default:
            __OUT();
    }
    return fopen(fileName, fileMode);
}

void FileStream::CreateFile(LPCSTR fileName) const {
    if (FileExists(fileName)) {
        return;
    }
    auto fp = fopen(fileName, "w");
    fclose(fp);
}
