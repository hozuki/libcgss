#pragma once

#include <vector>
#include <map>

#include "../cgss_env.h"
#include "CUtfTable.h"
#include "../cdata/ACB_CUE_RECORD.h"
#include "../cdata/ACB_TRACK_RECORD.h"

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

        IStream *OpenDataStream(const AFS2_FILE_RECORD *fileRecord, bool_t isStreaming) const;

        IStream *OpenDataStream(const char *fileName) const;

        IStream *OpenDataStream(uint32_t cueId) const;

        static std::string GetSymbolicFileNameFromCueId(uint32_t cueId);

        std::string GetSymbolicFileNameHintFromCueId(uint32_t cueId) const;

        std::string GetCueNameFromCueId(uint32_t cueId) const;

        const ACB_CUE_RECORD *GetCueRecordByWaveformFileName(const char *waveformFileName) const;

        const ACB_CUE_RECORD *GetCueRecordByCueId(uint32_t cueId) const;

        const AFS2_FILE_RECORD *GetFileRecordByWaveformFileName(const char *waveformFileName) const;

        const AFS2_FILE_RECORD *GetFileRecordByCueId(uint32_t cueId) const;

        const AFS2_FILE_RECORD *GetFileRecordByTrackIndex(uint32_t trackIndex) const;

        const std::vector<ACB_TRACK_RECORD> &GetTrackRecords() const;

        bool_t IsCueIdentified(uint32_t cueId) const;

        void Initialize() override;

        const CAfs2Archive *GetInternalAwb() const;

        const CAfs2Archive *GetExternalAwb() const;

        uint32_t GetFormatVersion() const;

        std::string GetFileExtensionHint(uint32_t cueId) const;

        std::string GetFileExtensionHint(const char *waveformFileName) const;

        static const uint32_t KEY_MODIFIER_ENABLED_VERSION;

    private:

        void InitializeCueList();

        void InitializeCueNameToWaveformMap();

        void InitializeTrackList();

        void InitializeAwbArchives();

        const AFS2_FILE_RECORD *GetFileRecordByCueRecord(const ACB_CUE_RECORD *cue) const;

        IStream *ChooseSourceStream(const ACB_CUE_RECORD *cue) const;

        std::string FindExternalAwbFileName() const;

        /**
         * You do not need to manually delete the pointer retrieved.
         * @param tableName
         */
        CUtfTable *GetTable(const char *tableName);

        CUtfTable *ResolveTable(const char *tableName) const;

        const CAfs2Archive *_internalAwb;
        const CAfs2Archive *_externalAwb;

        std::vector<std::string> _fileNames;
        std::vector<ACB_CUE_RECORD> _cues;
        std::vector<ACB_TRACK_RECORD> _tracks;

        std::map<std::string, uint16_t> _cueNameToWaveform;

        std::map<std::string, CUtfTable *> _tables;

        uint32_t _formatVersion;

        const char *_fileName;

    };

CGSS_NS_END
