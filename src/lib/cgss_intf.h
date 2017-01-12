#pragma once

#include "cgss_env.h"
#include "cgss_enum.h"

#include "takamori/exceptions/CException.h"
#include "takamori/streams/IStream.h"
#include "takamori/streams/CStream.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/CFileStream.h"
#include "takamori/streams/CBinaryReader.h"
#include "takamori/streams/CBinaryWriter.h"

#include "kawashima/hca/CHcaFormatReader.h"
#include "kawashima/hca/CDefaultWaveGenerator.h"
#include "kawashima/hca/CHcaDecoder.h"
#include "kawashima/hca/CHcaCipherConverter.h"
