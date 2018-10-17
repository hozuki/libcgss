#pragma once

#include <cstdio>
#include "CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CFileStream : public CStream {

    __extends(CStream, CFileStream);

    public:

        explicit CFileStream(LPCSTR fileName);

        CFileStream(LPCSTR fileName, FileMode mode);

        CFileStream(LPCSTR fileName, FileMode mode, FileAccess access);

        virtual ~CFileStream();

        virtual uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        virtual uint32_t Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        virtual bool_t IsWritable() const override;

        virtual bool_t IsReadable() const override;

        virtual bool_t IsSeekable() const override;

        virtual uint64_t GetPosition() override;

        virtual void SetPosition(uint64_t value) override;

        virtual uint64_t GetLength() override;

        virtual void SetLength(uint64_t value) override;

        virtual void Flush() override;

    protected:

        CFileStream() = default;

    private:

        CFileStream(const CFileStream &) = delete;

        FILE *OpenFile(LPCSTR fileName);

        void CreateFileInternal(LPCSTR fileName) const;

    private:

        FILE *_fp;
        FileMode _mode;
        FileAccess _access;
        bool_t _isReadable, _isWritable, _isSeekable;

    };

CGSS_NS_END
