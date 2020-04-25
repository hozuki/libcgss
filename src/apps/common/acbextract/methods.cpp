#include <cinttypes>
#include <unordered_set>
#include <vector>

#include "methods.h"

using namespace std;
using namespace cgss;

void ExtractFilesWithCueInfo(AcbWalkCallbackParams &p, CAcbFile &acb, AcbWalkOptions *options, const string &extractDir, const CAfs2Archive *archive, cgss::IStream *archiveDataStream) {
    unordered_set<uint32_t> extractedCueIds;

    // First try to extract files with readable cue names
    {
        const auto &fileNames = acb.GetFileNames();

        for (const auto &fileName : fileNames) {
            if (fileName.empty()) {
                continue;
            }

            auto entryDataStream = acb.OpenDataStream(fileName.c_str());

            if (entryDataStream) {
                p.entryDataStream = entryDataStream;

                const auto fileRecord = acb.GetFileRecordByWaveformFileName(fileName.c_str());

                if (fileRecord == nullptr) {
                    throw CException("Should not happen.");
                }

                string extractFileName;

                if (options->useCueName) {
                    extractFileName = fileName;
                } else {
                    extractFileName = acb.GetSymbolicFileNameHintByCueId(fileRecord->cueId);
                }

                if (options->prependId) {
                    auto prepend = CAcbFile::GetSymbolicFileBaseNameByCueId(fileRecord->cueId);

                    extractFileName.insert(0, "_");
                    extractFileName.insert(0, prepend);
                }

                p.extractPathHint = CPath::Combine(extractDir, extractFileName);

                p.cueInfo.id = fileRecord->cueId;
                p.cueInfo.offset = fileRecord->fileOffsetAligned;
                p.cueInfo.size = fileRecord->fileSize;

                options->callback(&p);

                extractedCueIds.insert(fileRecord->cueId);
            } else {
                fprintf(stderr, "Cue of '%s' cannot be retrieved.\n", fileName.c_str());
            }

            p.entryDataStream = nullptr;
            p.extractPathHint = "";

            delete entryDataStream;
        }
    }

    // Then try on not-yet-exported files
    {
        const auto &fileList = archive->GetFiles();

        for (auto &entry : fileList) {
            auto &record = entry.second;

            if (extractedCueIds.find(record.cueId) != extractedCueIds.end()) {
                continue;
            }

            string extractFileName;

            if (options->useCueName) {
                extractFileName = acb.GetCueNameByCueId(record.cueId);
            } else {
                extractFileName = acb.GetSymbolicFileNameHintByCueId(record.cueId);
            }

            if (options->prependId) {
                auto prepend = CAcbFile::GetSymbolicFileBaseNameByCueId(record.cueId);

                extractFileName.insert(0, "_");
                extractFileName.insert(0, prepend);
            }

            auto entryDataStream = CAcbHelper::ExtractToNewStream(archiveDataStream, record.fileOffsetAligned, (uint32_t)record.fileSize);

            p.entryDataStream = entryDataStream;
            p.extractPathHint = CPath::Combine(extractDir, extractFileName);
            p.cueInfo.id = record.cueId;
            p.cueInfo.offset = record.fileOffsetAligned;
            p.cueInfo.size = record.fileSize;

            options->callback(&p);

            p.entryDataStream = nullptr;
            p.extractPathHint = "";
            p.cueInfo.id = 0;
            p.cueInfo.offset = 0;
            p.cueInfo.size = 0;

            delete entryDataStream;
        }
    }
}

void ExtractFilesWithCueAndSequenceInfo(AcbWalkCallbackParams &p, CAcbFile &acb, AcbWalkOptions *options, const string &extractDir) {
    // Here we assume that all "meaningful" cues are named, so we don't export unnamed cues.
    const auto &fileNames = acb.GetFileNames();
    const auto &tracks = acb.GetTrackRecords();
    uint32_t i = 0;
    char trackIndexCounterTextBuffer[10] = {'\0'};
    char trackIndexTextBuffer[10] = {'\0'};

    for (const auto &fileName : fileNames) {
        if (fileName.empty()) {
            continue;
        }

        const auto cue = acb.GetCueRecordByWaveformFileName(fileName.c_str());

        if (cue == nullptr) {
            throw CException("Should not happen.");
        }

        auto extensionHint = acb.GetFileExtensionHintByCueId(cue->cueId);

        string extractCueGroupName;

        if (options->useCueName) {
            extractCueGroupName = CPath::GetFileBaseName(fileName);
        } else {
            extractCueGroupName = CAcbFile::GetSymbolicFileBaseNameByCueId(cue->cueId);
        }

        if (options->prependId) {
            auto prepend = CAcbFile::GetSymbolicFileBaseNameByCueId(cue->cueId);

            extractCueGroupName.insert(0, "_");
            extractCueGroupName.insert(0, prepend);
        }

        vector<uint32_t> trackIndices;
        acb.GetTrackIndicesOfCueByCueId(cue->cueId, trackIndices);

        if (trackIndices.empty()) {
            throw CException("Should not happen.");
        }

        uint32_t trackIndexCounter = 0;

        for (const auto trackIndex : trackIndices) {
            if (trackIndex >= tracks.size()) {
                fprintf(stderr, "Invalid track index '%" PRIu32 "' for cue %" PRIu32 " (%s).\n", trackIndex, cue->cueId, fileName.c_str());
                continue;
            }

            const auto fileRecord = acb.GetFileRecordByTrackIndex(trackIndex);

            if (fileRecord == nullptr) {
                fprintf(stderr, "AFS file record is not found for track #%" PRIu32 ".\n", i + 1);
                continue;
            }

            snprintf(trackIndexCounterTextBuffer, sizeof(trackIndexCounterTextBuffer), "%04" PRIu32, trackIndexCounter);
            snprintf(trackIndexTextBuffer, sizeof(trackIndexTextBuffer), "%06" PRIu32, trackIndex);

            string extractFileName = extractCueGroupName;
            extractFileName.append("_");
            extractFileName.append(trackIndexCounterTextBuffer);
            extractFileName.append("_");
            extractFileName.append(trackIndexTextBuffer);
            extractFileName.append(extensionHint);

            auto entryDataStream = acb.OpenDataStream(fileRecord, cue->isStreaming);

            if (entryDataStream != nullptr) {
                p.entryDataStream = entryDataStream;
                p.extractPathHint = CPath::Combine(extractDir, extractFileName);

                p.cueInfo.id = cue->cueId;
                p.cueInfo.offset = fileRecord->fileOffsetAligned;
                p.cueInfo.size = fileRecord->fileSize;

                options->callback(&p);
            } else {
                fprintf(stderr, "Track #%" PRIu32 " cannot be retrieved.\n", trackIndex);
            }

            delete entryDataStream;

            trackIndexCounter += 1;
        }
    }

    p.entryDataStream = nullptr;
    p.extractPathHint = "";
}

void ExtractFilesWithTrackInfo(AcbWalkCallbackParams &p, CAcbFile &acb, AcbWalkOptions *options, const string &extractDir) {
    const auto &tracks = acb.GetTrackRecords();
    const auto trackCount = tracks.size();

    for (uint32_t i = 0; i < trackCount; i += 1) {
        const auto &track = tracks[i];

        const auto fileRecord = acb.GetFileRecordByTrackIndex(track.trackIndex);

        if (fileRecord == nullptr) {
            fprintf(stderr, "AFS file record is not found for track #%" PRIu32 ".\n", i + 1);
            continue;
        }

        auto entryDataStream = acb.OpenDataStream(fileRecord, track.isStreaming);

        if (entryDataStream) {
            p.entryDataStream = entryDataStream;

            string extractFileName;

            if (options->useCueName) {
                extractFileName = acb.GetCueNameByTrackIndex(track.trackIndex);
            } else {
                extractFileName = acb.GetSymbolicFileNameHintByTrackIndex(track.trackIndex);
            }

            if (options->prependId) {
                auto prepend = CAcbFile::GetSymbolicFileBaseNameByTrackIndex(track.trackIndex);

                extractFileName.insert(0, "_");
                extractFileName.insert(0, prepend);
            }

            p.extractPathHint = CPath::Combine(extractDir, extractFileName);

            p.cueInfo.id = fileRecord->cueId;
            p.cueInfo.offset = fileRecord->fileOffsetAligned;
            p.cueInfo.size = fileRecord->fileSize;

            options->callback(&p);
        } else {
            fprintf(stderr, "Track #%" PRIu32 " cannot be retrieved.\n", i + 1);
        }

        p.entryDataStream = nullptr;
        p.extractPathHint = "";

        delete entryDataStream;
    }
}

