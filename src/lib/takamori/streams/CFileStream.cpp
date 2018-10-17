#include <stdlib.h>
#include <stdio.h>

#include "../CFileSystem.h"

#ifndef __MINGW_H

#include <algorithm>

#endif

using namespace std;

#include "CFileStream.h"
#include "../exceptions/CArgumentException.h"
#include "../exceptions/CInvalidOperationException.h"

#define __FILE_READ ("rb")
#define __FILE_WRITE ("wb")
#define __FILE_APPEND ("ab")
#define __FILE_READ_U ("rb+")
#define __FILE_WRITE_U ("wb+")

CGSS_NS_BEGIN

    CFileStream::CFileStream(LPCSTR fileName)
        : MyClass(fileName, FileMode::OpenOrCreate) {
    }

    CFileStream::CFileStream(LPCSTR fileName, FileMode mode)
        : MyClass(fileName, mode, FileAccess::ReadWrite) {
    }

    CFileStream::CFileStream(LPCSTR fileName, FileMode mode, FileAccess access) {
        _mode = mode;
        _access = access;
        _fp = OpenFile(fileName);
    }

    CFileStream::~CFileStream() {
        if (_fp) {
            fclose(_fp);
        }
        _fp = nullptr;
    }

    uint32_t CFileStream::Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        if (!buffer) {
            throw CArgumentException("FileStream::Read()");
        }
        if (!IsReadable()) {
            throw CInvalidOperationException("FileStream::Read()");
        }
        const auto actualCount = min(static_cast<uint32_t>(bufferSize - offset), count);
        const auto byteBuffer = static_cast<uint8_t *>(buffer);
        const auto actualRead = fread(byteBuffer + offset, 1, actualCount, _fp);
        return static_cast<uint32_t>(actualRead);
    }

    uint32_t CFileStream::Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        if (!buffer) {
            throw CArgumentException("FileStream::Write()");
        }
        if (!IsWritable()) {
            throw CInvalidOperationException("FileStream::Write()");
        }
        const auto actualCount = min(static_cast<uint32_t>(bufferSize - offset), count);
        const auto byteBuffer = static_cast<const uint8_t *>(buffer);
        fwrite(byteBuffer + offset, 1, actualCount, _fp);
        return actualCount;
    }

    bool_t CFileStream::IsWritable() const {
        return _isWritable;
    }

    bool_t CFileStream::IsReadable() const {
        return _isReadable;
    }

    bool_t CFileStream::IsSeekable() const {
        return _isSeekable;
    }

    uint64_t CFileStream::GetPosition() {
        if (_fp == nullptr) {
            return 0;
        } else {
            return (uint64_t)ftell(_fp);
        }
    }

    void CFileStream::SetPosition(uint64_t value) {
        fseek(_fp, (long)value, SEEK_SET);
    }

    uint64_t CFileStream::GetLength() {
        if (_fp == nullptr) {
            return 0;
        }

        auto position = ftell(_fp);
        fseek(_fp, 0, SEEK_END);
        auto r = ftell(_fp);
        fseek(_fp, position, SEEK_SET);
        return (uint64_t)r;
    }

    void CFileStream::SetLength(uint64_t value) {
        fseek(_fp, (long)value, SEEK_SET);
    }

    void CFileStream::Flush() {
        fflush(_fp);
    }

    FILE *CFileStream::OpenFile(LPCSTR fileName) {
#define __OUT() throw CException("Mode/Access: out of range")
#define __CMB() throw CException("Mode/Access: incompatible")
#define __EXT() throw CException("File exists: " + std::string(fileName))
#define __NEX() throw CException("File doesn't exist: " + std::string(fileName))
        auto m = _mode;
        auto a = _access;
        // CLion cannot recognize bitwise operators defined. GCC can. However the error prompted by CLion is still annoying.
        _isReadable = (bool_t)(static_cast<std::underlying_type_t<FileAccess>>(a) & static_cast<std::underlying_type_t<FileMode>>(FileAccess::Read));
        _isWritable = (bool_t)(static_cast<std::underlying_type_t<FileAccess>>(a) & static_cast<std::underlying_type_t<FileMode>>(FileAccess::Write));
        _isSeekable = (bool_t)(m != FileMode::Append);
        if (m == FileMode::None || a == FileAccess::None) {
            __OUT();
        }
        LPCSTR fileMode = nullptr;
        switch (m) {
            case FileMode::Append:
                switch (a) {
                    case FileAccess::Write:
                        fileMode = __FILE_APPEND;
                        break;
                    case FileAccess::Read:
                    case FileAccess::ReadWrite:
                        __CMB();
                    default:
                        __OUT();
                }
                break;
            case FileMode::Create:
                switch (a) {
                    case FileAccess::Write:
                        fileMode = __FILE_WRITE_U;
                        break;
                    case FileAccess::Read:
                        CreateFileInternal(fileName);
                        fileMode = __FILE_READ;
                        break;
                    case FileAccess::ReadWrite:
                        CreateFileInternal(fileName);
                        fileMode = __FILE_READ_U;
                        break;
                    default:
                        __OUT();
                }
                break;
            case FileMode::CreateNew:
                if (CFileSystem::FileExists(fileName)) {
                    __EXT();
                }
                switch (a) {
                    case FileAccess::Write:
                    case FileAccess::Read:
                    case FileAccess::ReadWrite:
                        fileMode = __FILE_WRITE_U;
                        break;
                    default:
                        __OUT();
                }
                break;
            case FileMode::OpenExisting:
                if (!CFileSystem::FileExists(fileName)) {
                    __NEX();
                }
                switch (a) {
                    case FileAccess::Write:
                        fileMode = __FILE_WRITE;
                        break;
                    case FileAccess::Read:
                        fileMode = __FILE_READ;
                        break;
                    case FileAccess::ReadWrite:
                        fileMode = __FILE_READ_U;
                        break;
                    default:
                        __OUT();
                }
                break;
            case FileMode::OpenOrCreate:
                if (!CFileSystem::FileExists(fileName)) {
                    CreateFileInternal(fileName);
                }
                switch (a) {
                    case FileAccess::Write:
                        fileMode = __FILE_WRITE;
                        break;
                    case FileAccess::Read:
                        fileMode = __FILE_READ;
                        break;
                    case FileAccess::ReadWrite:
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

    void CFileStream::CreateFileInternal(LPCSTR fileName) const {
        if (CFileSystem::FileExists(fileName)) {
            return;
        }
        auto fp = fopen(fileName, "w");
        fclose(fp);
    }

CGSS_NS_END
