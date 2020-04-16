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

        std::string GetSymbolicFileNameHintFromCueId(uint32_t cueId);

        std::string GetCueNameFromCueId(uint32_t cueId);

        const ACB_CUE_RECORD *GetCueRecord(const char *waveformFileName);

        const ACB_CUE_RECORD *GetCueRecord(uint32_t cueId);

        const AFS2_FILE_RECORD *GetFileRecord(const char *waveformFileName);

        const AFS2_FILE_RECORD *GetFileRecord(uint32_t cueId);

        bool_t IsCueIdentified(uint32_t cueId);

        void Initialize() override;

        CAfs2Archive *GetInternalAwb();

        CAfs2Archive *GetExternalAwb();

        uint32_t GetFormatVersion() const;

        std::string GetFileExtensionHint(uint32_t cueId);

        std::string GetFileExtensionHint(const char *waveformFileName);

        static const uint32_t KEY_MODIFIER_ENABLED_VERSION;

    private:

        void InitializeAcbTables();

        void InitializeCueNameToWaveformTable();

        void InitializeAwbArchives();

        const AFS2_FILE_RECORD *GetFileRecord(const ACB_CUE_RECORD *cue);

        IStream *ChooseSourceStream(const ACB_CUE_RECORD *cue);

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
