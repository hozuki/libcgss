#pragma once

#include "cgss_env.h"
#include "cgss_cenum.h"
#include "cgss_cdata.h"

// C API
typedef uint32_t CGSS_HANDLE;

CGSS_API_DECL(void) cgssTest();

CGSS_API_DECL(void) cgssGetOpResultString(CGSS_OP_RESULT error, char *buffer, size_t length);
CGSS_API_DECL(const char *) cgssGetLastErrorMessage();

CGSS_API_DECL(bool_t) cgssHelperFileExists(LPCSTR fileName);

CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamRead(CGSS_HANDLE handle, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *read);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamWrite(CGSS_HANDLE handle, void *buffer, uint32_t bufferSize, size_t offset, uint32_t count, _OUT_ uint32_t *written);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamSeek(CGSS_HANDLE handle, int64_t offset, CGSS_STREAM_SEEK_ORIGIN origin);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamIsReadable(CGSS_HANDLE handle, _OUT_ bool_t *isReadable);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamIsWritable(CGSS_HANDLE handle, _OUT_ bool_t *isWritable);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamIsSeekable(CGSS_HANDLE handle, _OUT_ bool_t *isSeekable);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamGetPosition(CGSS_HANDLE handle, _OUT_ uint64_t *position);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamSetPosition(CGSS_HANDLE handle, uint64_t position);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamGetLength(CGSS_HANDLE handle, _OUT_ uint64_t *length);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamSetLength(CGSS_HANDLE handle, uint64_t length);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamReadByte(CGSS_HANDLE handle, _OUT_ uint8_t *byte);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamWriteByte(CGSS_HANDLE handle, uint8_t byte);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamFlush(CGSS_HANDLE handle);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamCopyTo(CGSS_HANDLE source, CGSS_HANDLE destination);
CGSS_API_DECL(CGSS_OP_RESULT) cgssStreamCopyTo2(CGSS_HANDLE source, CGSS_HANDLE destination, uint32_t bufferSize);

CGSS_API_DECL(CGSS_OP_RESULT) cgssCloseHandle(CGSS_HANDLE handle);

CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateFileStream(LPCSTR fileName, _OUT_ CGSS_HANDLE *stream);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateFileStream2(LPCSTR fileName, CGSS_FILE_MODE fileMode, _OUT_ CGSS_HANDLE *stream);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateFileStream3(LPCSTR fileName, CGSS_FILE_MODE fileMode, CGSS_FILE_ACCESS fileAccess, _OUT_ CGSS_HANDLE *stream);

CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateHcaDecoder(CGSS_HANDLE baseStream, _OUT_ CGSS_HANDLE *decoder);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateHcaDecoder2(CGSS_HANDLE baseStream, const HCA_DECODER_CONFIG *decoderConfig, _OUT_ CGSS_HANDLE *decoder);
CGSS_API_DECL(CGSS_OP_RESULT) cgssCreateCipherConverter(CGSS_HANDLE baseStream, const HCA_CIPHER_CONFIG *cryptFrom, const HCA_CIPHER_CONFIG *cryptTo, _OUT_ CGSS_HANDLE *converter);

CGSS_API_DECL(CGSS_OP_RESULT) cgssGetHcaInfo(CGSS_HANDLE handle, HCA_INFO *info);

CGSS_API_DECL(uint32_t) cgssWaveDecode8BitU(float data, uint8_t *buffer, uint32_t cursor);
CGSS_API_DECL(uint32_t) cgssWaveDecode16BitS(float data, uint8_t *buffer, uint32_t cursor);
CGSS_API_DECL(uint32_t) cgssWaveDecode24BitS(float data, uint8_t *buffer, uint32_t cursor);
CGSS_API_DECL(uint32_t) cgssWaveDecode32BitS(float data, uint8_t *buffer, uint32_t cursor);
CGSS_API_DECL(uint32_t) cgssWaveDecodeFloat(float data, uint8_t *buffer, uint32_t cursor);

CGSS_API_DECL(CGSS_OP_RESULT) cgssUtfReadTable(CGSS_HANDLE stream, uint64_t offset, _OUT_ UTF_TABLE **table);
CGSS_API_DECL(CGSS_OP_RESULT) cgssUtfFreeTable(UTF_TABLE *table);
CGSS_API_DECL(bool_t) cgssUtfTryParseTable(void *data, size_t dataSize, _OUT_ UTF_TABLE **table);
