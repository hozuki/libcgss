#pragma once

#include "cgss_env.h"
#include "cgss_enum.h"

#include "takamori/exceptions/CException.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/exceptions/CNotImplementedException.h"
#include "takamori/exceptions/CFormatException.h"

#include "takamori/streams/IStream.h"
#include "takamori/streams/CStream.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/CFileStream.h"
#include "takamori/streams/CBinaryReader.h"
#include "takamori/streams/CBinaryWriter.h"

#include "takamori/CBitConverter.h"
#include "takamori/CPath.h"
#include "takamori/CFileSystem.h"

#include "kawashima/hca/CHcaFormatReader.h"
#include "kawashima/hca/CDefaultWaveGenerator.h"
#include "kawashima/hca/CHcaDecoder.h"
#include "kawashima/hca/CHcaCipherConverter.h"

#include "ichinose/CAcbHelper.h"
#include "ichinose/CUtfField.h"
#include "ichinose/CUtfReader.h"
#include "ichinose/CUtfTable.h"
#include "ichinose/CAfs2Archive.h"
#include "ichinose/CAcbFile.h"
