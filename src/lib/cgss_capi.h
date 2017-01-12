#pragma once

#include "cgss_env.h"
#include "cgss_cenum.h"
#include "cgss_cdata.h"

// C API
typedef void *CGSS_HSTREAM;

CGSS_API_DECL(void) cgssTest();

CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamRead(CGSS_HSTREAM stream, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *read);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamWrite(CGSS_HSTREAM stream, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *written);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamSeek(CGSS_HSTREAM stream, int64_t offset, CGSS_STREAM_SEEK_ORIGIN origin);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamIsReadable(CGSS_HSTREAM stream, _OUT_ bool_t *isReadable);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamIsWritable(CGSS_HSTREAM stream, _OUT_ bool_t *isWritable);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamIsSeekable(CGSS_HSTREAM stream, _OUT_ bool_t *isSeekable);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamGetPosition(CGSS_HSTREAM stream, _OUT_ uint64_t *position);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamSetPosition(CGSS_HSTREAM stream, uint64_t position);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamGetLength(CGSS_HSTREAM stream, _OUT_ uint64_t *length);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamSetLength(CGSS_HSTREAM stream, uint64_t length);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamReadByte(CGSS_HSTREAM stream, _OUT_ uint8_t *byte);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamWriteByte(CGSS_HSTREAM stream, uint8_t byte);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamFlush(CGSS_HSTREAM stream);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamCopyTo(CGSS_HSTREAM source, CGSS_HSTREAM destination);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamCopyTo2(CGSS_HSTREAM source, CGSS_HSTREAM destination, uint32_t bufferSize);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCloseStream(CGSS_HSTREAM stream);

CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateFileStream(LPCSTR fileName, _OUT_ CGSS_HSTREAM **stream);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateFileStream2(LPCSTR fileName, CGSS_FILE_MODE fileMode, _OUT_ CGSS_HSTREAM **stream);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateFileStream3(LPCSTR fileName, CGSS_FILE_MODE fileMode, CGSS_FILE_ACCESS fileAccess, _OUT_ CGSS_HSTREAM **stream);

CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateHcaDecoder(CGSS_HSTREAM baseStream, _OUT_ CGSS_HSTREAM **decoder);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateHcaDecoder2(CGSS_HSTREAM baseStream, const HCA_DECODER_CONFIG *decoderConfig, _OUT_ CGSS_HSTREAM **decoder);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateCipherConverter(CGSS_HSTREAM baseStream, const HCA_CIPHER_CONFIG *cryptFrom, const HCA_CIPHER_CONFIG *cryptTo, _OUT_ CGSS_HSTREAM **converter);

CGSS_API_DECL(void) cgssWaveDecode8BitU(float data, uint8_t *buffer, uint32_t *cursor);
CGSS_API_DECL(void) cgssWaveDecode16BitS(float data, uint8_t *buffer, uint32_t *cursor);
CGSS_API_DECL(void) cgssWaveDecode24BitS(float data, uint8_t *buffer, uint32_t *cursor);
CGSS_API_DECL(void) cgssWaveDecode32BitS(float data, uint8_t *buffer, uint32_t *cursor);
CGSS_API_DECL(void) cgssWaveDecodeFloat(float data, uint8_t *buffer, uint32_t *cursor);
