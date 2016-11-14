#ifndef CGSS_FILESTREAM_H
#define CGSS_FILESTREAM_H

#include "tm_stream.h"

class FileStream final : public Stream {

public:
    enum FileAccess {
        ACCESS_NONE = 0,
        ACCESS_READ = 1,
        ACCESS_WRITE = 2,
        ACCESS_READ_WRITE = 3
    };

    enum FileMode {
        MODE_NONE = 0,
        MODE_APPEND = 1,
        MODE_CREATE = 2,
        MODE_CREATE_NEW = 3,
        MODE_OPEN_EXISTING = 4,
        MODE_OPEN_OR_CREATE = 5
    };

public:

    FileStream(LPCSTR fileName);

    FileStream(LPCSTR fileName, FileMode mode);

    FileStream(LPCSTR fileName, FileMode mode, FileAccess access);

    ~FileStream();

    virtual uint32 Read(uint8 *buffer, uint32 bufferSize, size_t offset, uint32 count) override;

    virtual uint32 Write(const uint8 *buffer, uint32 bufferSize, uint32 offset, uint32 count) override;

    virtual ubool IsWritable() const override;

    virtual ubool IsReadable() const override;

    virtual ubool IsSeekable() const override;

    virtual uint64 GetPosition() const override;

    virtual void SetPosition(uint64 value) override;

    virtual uint64 GetLength() const override;

    virtual void SetLength(uint64 value) override;

    virtual void Flush() override;

    static ubool FileExists(LPCSTR fileName);

private:

    FILE *OpenFile(LPCSTR fileName);

#undef CreateFile

    void CreateFile(LPCSTR fileName) const;

    FileMode _mode;
    FileAccess _access;
    FILE *_fp;
    ubool _isReadable, _isWritable, _isSeekable;

};

#endif //CGSS_FILESTREAM_H
