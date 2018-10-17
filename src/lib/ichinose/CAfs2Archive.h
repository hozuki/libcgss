#pragma once

#include <map>
#include "../cgss_env.h"
#include "../cdata/AFS2_FILE_RECORD.h"

CGSS_NS_BEGIN

    struct IStream;

    class CGSS_EXPORT CAfs2Archive final {

    __root_class(CAfs2Archive);

    public:

        CAfs2Archive(IStream *stream, uint64_t offset, const char *fileName, bool_t disposeStream);

        ~CAfs2Archive();

        static bool_t IsAfs2Archive(IStream *stream, uint64_t offset);

        const std::map<uint32_t, AFS2_FILE_RECORD> &GetFiles() const;

        uint32_t GetByteAlignment() const;

        uint32_t GetVersion() const;

        uint16_t GetHcaKeyModifier() const;

        const char *GetFileName() const;

    private:

        void Initialize();

        IStream *_stream;
        uint64_t _streamOffset;
        char *_fileName;
        bool_t _disposeStream;

        std::map<uint32_t, AFS2_FILE_RECORD> _files;

        uint32_t _byteAlignment;
        uint16_t _hcaKeyModifier;
        uint32_t _version;

    };

CGSS_NS_END
