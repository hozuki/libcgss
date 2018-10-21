#pragma once

#include <vector>
#include <map>
#include "../cgss_env.h"
#include "CUtfTable.h"
#include "../cdata/ACB_CUE_RECORD.h"

CGSS_NS_BEGIN

    struct IStream;

    class CAfs2Archive;

    class CGSS_EXPORT CAcbFile final : public CUtfTable {

    __extends(CUtfTable, CAcbFile);

    public:

        CAcbFile(IStream *stream, const char *fileName);

        CAcbFile(IStream *stream, uint64_t streamOffset, const char *fileName);

        virtual ~CAcbFile();

        const std::vector<std::string> &GetFileNames() const;

        const char *GetFileName() const;

        IStream *OpenDataStream(const char *fileName);

        IStream *OpenDataStream(uint32_t cueId);

        static std::string GetSymbolicFileNameFromCueId(uint32_t cueId);

        std::string GetCueNameFromCueId(uint32_t cueId);

        void Initialize() override;

        CAfs2Archive *GetInternalAwb();

        CAfs2Archive *GetExternalAwb();

        uint32_t GetFormatVersion() const;

        static const uint32_t KEY_MODIFIER_ENABLED_VERSION;

    private:

        void InitializeAcbTables();

        void InitializeCueNameToWaveformTable();

        void InitializeAwbArchives();

        IStream *GetDataStreamFromCueInfo(const ACB_CUE_RECORD &cue, const char *fileNameForError);

        std::string FindExternalAwbFileName();

        /**
         * You do not need to manually delete the pointer retrieved.
         * @param tableName
         */
        CUtfTable *GetTable(const char *tableName);

        CUtfTable *ResolveTable(const char *tableName);

        CAfs2Archive *_internalAwb;
        CAfs2Archive *_externalAwb;

        std::vector<std::string> _fileNames;
        std::vector<ACB_CUE_RECORD> _cues;
        std::map<std::string, CUtfTable *> _tables;
        std::map<std::string, uint16_t> _cueNameToWaveform;

        uint32_t _formatVersion;

        const char *_fileName;

    };

CGSS_NS_END
