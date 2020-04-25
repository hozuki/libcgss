#pragma once

#include <cstdio>
#include <string>

#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CFileStream final : public CStream {

    __extends(CStream, CFileStream);

    public:

        explicit CFileStream(const char *fileName);

        CFileStream(const char *fileName, FileMode mode);

        CFileStream(const char *fileName, FileMode mode, FileAccess access);

        explicit CFileStream(const std::string &fileName);

        CFileStream(const std::string &fileName, FileMode mode);

        CFileStream(const std::string &fileName, FileMode mode, FileAccess access);

        CFileStream(const CFileStream &) = delete;

        CFileStream(CFileStream &&) = delete;

        CFileStream &operator=(const CFileStream &) = delete;

        CFileStream &operator=(CFileStream &&) = delete;

        ~CFileStream() override;

        uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        uint32_t Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        bool_t IsWritable() const override;

        bool_t IsReadable() const override;

        bool_t IsSeekable() const override;

        uint64_t GetPosition() override;

        void SetPosition(uint64_t value) override;

        uint64_t GetLength() override;

        void SetLength(uint64_t value) override;

        void Flush() override;

    private:

        FILE *OpenFile(const char *fileName);

        static void CreateFileInternal(const char *fileName);

    private:

        FILE *_fp;
        FileMode _mode;
        FileAccess _access;
        bool_t _isReadable, _isWritable, _isSeekable;

    };

CGSS_NS_END
