#pragma once

#include <string>

#include "../acbextract.h"

void ExtractFilesWithCueInfo(AcbWalkCallbackParams &p, cgss::CAcbFile &acb, AcbWalkOptions *options, const std::string &extractDir, const cgss::CAfs2Archive *archive, cgss::IStream *archiveDataStream);

void ExtractFilesWithCueAndSequenceInfo(AcbWalkCallbackParams &p, cgss::CAcbFile &acb, AcbWalkOptions *options, const std::string &extractDir);

void ExtractFilesWithTrackInfo(AcbWalkCallbackParams &p, cgss::CAcbFile &acb, AcbWalkOptions *options, const std::string &extractDir);
