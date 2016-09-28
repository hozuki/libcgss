# kawashima API

Version: v0.1

- [Functions](#functions)
- [Structures](#structures)
- [Enums](#enums)
- [Primitive Types](#primitive-types)

## Functions

- [KsOpenFile](#ksopenfile)
- [KsOpenBuffer](#ksopenbuffer)
- [KsSetParamI32](#kssetparami32)
- [KsSetParamI64](#kssetparami64)
- [KsBeginDecode](#ksbegindecode)
- [KsGetWaveHeader](#ksgetwaveheader)
- [KsDecodeData](#ksdecodedata)
- [KsEndDecode](#ksenddecode)
- [KsCloseHandle](#ksclosehandle)
- [KsGetHcaInfo](#ksgethcainfo)
- [KsIsActiveHandle](#ksisactivehandle)
- [KsIsHcaCheckPassed](#ksishcacheckpassed)
- [KsHasMoreData](#kshasmoredata)

### KsOpenFile

**Declaration:**

```cpp
KS_API KS_RESULT KsOpenFile(const char *pFileName, HKDECODE *ppHandle);
```

**Description:**

Opens an HCA file and returns its decoder handle.

**Parameters:**

- `pFileName`: name of the HCA file.
- `ppHandle`: pointer to an `HKDECODE`, to receive the decoder handle if succeeded.

### KsOpenBuffer

**Declaration:**

```cpp
KS_API KS_RESULT KsOpenBuffer(uint8 *pData, uint32 dwDataSize, ubool bClone, HKDECODE *ppHandle);
```

**Description:**

Uses the data in a buffer and creates a decoder handle for that data.

**Parameters:**

- `pData`: pointer to the data buffer.
- `dwDataSize`: size of data, in bytes.
- `bClone`: whether to reuse the original buffer, or to copy the data to a new buffer. If you reuse the
original buffer, please keep the data in it unchanged until the whole decoding process is finished.
- `ppHandle`: pointer to an `HKDECODE`, to receive the decoder handle if succeeded.

### KsSetParamI32

**Declaration:**

```cpp
KS_API KS_RESULT KsSetParamI32(HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint32 dwParam);
```

**Description:**

Sets a 32-bit decoding parameter before starting to decode.

**Parameters:**

- `hDecode`: the decoder handle.
- `dwParamType`: the decoding parameter you want to set.
- `dwParam`: the new value of the parameter.

### KsSetParamI64

**Declaration:**

```cpp
KS_API KS_RESULT KsSetParamI64(HKDECODE hDecode, KS_PARAM_TYPE dwParamType, uint64 qwParam);
```

**Description:**

Sets a 64-bit decoding parameter before starting to decode.

**Parameters:**

- `hDecode`: the decoder handle.
- `dwParamType`: the decoding parameter you want to set.
- `qwParam`: the new value of the parameter.

### KsBeginDecode

**Declaration:**

```cpp
KS_API KS_RESULT KsBeginDecode(HKDECODE hDecode);
```

**Description:**

Prepares the decoder for the decoding process.

**Parameters:**

- `hDecode`: the decoder handle.

### KsGetWaveHeader

**Declaration:**

```cpp
KS_API KS_RESULT KsGetWaveHeader(HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize);
```

**Description:**

Gets the wave header generated from audio description in the HCA file.

**Parameters:**

- `hDecode`: the decoder handle.
- `pBuffer`: the data buffer.
- `pdwDataSize`: a pointer to a `uint32` to specify the buffer size, and to receive actual read size.

**Remarks:**

You can pass `NULL` to `pBuffer`. In this case, `pdwDataSize` will return the actual size of generated WAVE header.
When an HCA file is set, the WAVE header size is determined.

### KsDecodeData

**Declaration:**

```cpp
KS_API KS_RESULT KsDecodeData(HKDECODE hDecode, uint8 *pBuffer, uint32 *pdwDataSize);
```

**Description:**

Decodes data blocks from HCA to WAVE.

**Parameters:**

- `hDecode`: the decoder handle.
- `pBuffer`: the data buffer.
- `pdwDataSize`: a pointer to a `uint32` to specify the buffer size, and to receive actual read size.

**Remarks:**

You can pass `NULL` to `pBuffer`. In this case, `pdwDataSize` will return the size of one generated WAVE data block.
When the buffer is not `NULL`, the decoder will try to decode as many blocks as possible. For example, given a buffer
of the size of 3.7 blocks, the decoder will decode 3 blocks if there is more than 2 blocks left.

### KsEndDecode

**Declaration:**

```cpp
KS_API KS_RESULT KsEndDecode(HKDECODE hDecode);
```

**Description:**

Ends the decode process. After this function is called, no more decoding operation can be performed.

**Parameters:**

- `hDecode`: the decoder handle.

### KsCloseHandle

**Declaration:**

```cpp
KS_API KS_RESULT KsCloseHandle(HKDECODE hDecode);
```

**Description:**

Closes the decoder handle, and frees all resources used by the decoder.

**Parameters:**

- `hDecode`: the decoder handle.

### KsGetHcaInfo

**Declaration:**

```cpp
KS_API KS_RESULT KsGetHcaInfo(HKDECODE hDecode, HCA_INFO *pInfo);
```

**Description:**

Gets key information of the HCA file. This function can only be called after the decoding process has begun
(`KsBeginDecode` is called).

**Parameters:**

- `hDecode`: the decoder handle.
- `pInfo`: a pointer to an `HCA_INFO` structure to receive key information of this HCA file.

### KsIsActiveHandle

**Declaration:**

```cpp
KS_API ubool KsIsActiveHandle(HKDECODE hDecode);
```

**Description:**

Checks whether a decoder handle is still active.

**Parameters:**

- `hDecode`: the decoder handle.

### KsIsHcaCheckPassed

**Declaration:**

```cpp
KS_API ubool KsIsHcaCheckPassed(HKDECODE hDecode);
```

**Description:**

Checks whether the current decoder has validated the HCA file and the file is valid.

**Parameters:**

- `hDecode`: the decoder handle.

### KsHasMoreData

**Declaration:**

```cpp
KS_API KS_RESULT KsHasMoreData(HKDECODE hDecode, ubool *pbHasMore);
```

**Description:**

Checks whether there is more data to be decoded.

**Parameters:**

- `hDecode`: the decoder handle.
- `pbHasMore`: a pointer to a `ubool` to receive whether there is more data to be decoded.

## Structures

### HCA_INFO

**Definition:**

```cpp
typedef struct _HCA_INFO {
    uint16 versionMajor;
    uint16 versionMinor;
    uint32 channelCount;
    uint32 samplingRate;
    uint32 blockCount;
    uint16 blockSize;
    uint16 athType;
    ubool loopExists;
    uint32 loopStart, loopEnd;
    HCA_CIPHER_TYPE cipherType;
    float rvaVolume;
    uint8 commentLength;
    char comment[0x100];
    uint16 fmtR01, fmtR02;
    uint16 compR01, compR02, compR03, compR04, compR05, compR06, compR07, compR08;
    uint32 compR09;
    uint16 vbrR01, vbrR02;
    uint16 loopR01, loopR02;
} HCA_INFO;
```

For full descriptions, please see [hca_info.h](https://github.com/Hozuki/kawashima/tree/master/src/hca_info.h).

## Enums

### KS_RESULT

**Definition:**

```cpp
typedef enum {
    KS_OP_HAS_MORE_DATA = 1,
    KS_ERR_OK = 0,
    KS_ERR_INVALID_HANDLE = (int32)-1,
    KS_ERR_MAGIC_NOT_MATCH = (int32)-2,
    KS_ERR_ALREADY_CLOSED = (int32)-3,
    KS_ERR_INVALID_PARAMETER = (int32)-4,
    KS_ERR_INVALID_STAGE = (int32)-5,
    KS_ERR_FILE_OP_FAILED = (int32)-6,
    KS_ERR_INVALID_FILE_PROP = (int32)-7,
    KS_ERR_INVALID_OPERATION = (int32)-8,
    KS_ERR_DECODE_FAILED = (int32)-9,
    KS_ERR_BUFFER_TOO_SMALL = (int32)-10,
    KS_ERR_CHECKSUM_NOT_MATCH = (int32)-11,
    KS_ERR_INVALID_INTERNAL_STATE = (int32)-12,
    KS_ERR_ATH_INIT_FAILED = (int32)-13,
    KS_ERR_CIPH_INIT_FAILED = (int32)-14,
    KS_ERR_STATE_OUT_OF_RANGE = (int32)-15,
    KS_ERR_NOT_IMPLEMENTED = (int32)-16,
    KS_ERR_DECODE_ALREADY_COMPLETED = (int32)-17
} KS_RESULT;
```

**Description:**

- `KS_OP_HAS_MORE_DATA`: there is more data to be decoded.
- `KS_ERR_OK`: the operation has been completed successfully.
- `KS_ERR_INVALID_HANDLE`: the handle value is invalid.
- `KS_ERR_MAGIC_NOT_MATCH`: the handle magic value, or HCA header magic values do not match during validation.
- `KS_ERR_ALREADY_CLOSED`: the handle is already closed.
- `KS_ERR_INVALID_PARAMETER`: one of the parameters provided is invalid.
- `KS_ERR_INVALID_STAGE`: unused.
- `KS_ERR_FILE_OP_FAILED`: file operations failed.
- `KS_ERR_INVALID_FILE_PROP`: the file properties are invalid.
- `KS_ERR_INVALID_OPERATION`: cannot perform desired operation due to various reasons.
- `KS_ERR_DECODE_FAILED`: unused.
- `KS_ERR_BUFFER_TOO_SMALL`: the given buffer is too small for outcoming data.
- `KS_ERR_CHECKSUM_NOT_MATCH`: the checksum of HCA data does not match during validation.
- `KS_ERR_INVALID_INTERNAL_STATE`: internal state is invalid.
- `KS_ERR_ATH_INIT_FAILED`: for debug only.
- `KS_ERR_CIPH_INIT_FAILED`: for debug only.
- `KS_ERR_STATE_OUT_OF_RANGE`: current state is out of possible range.
- `KS_ERR_NOT_IMPLEMENTED`: the required function is not implemented yet.
- `KS_ERR_DECODE_ALREADY_COMPLETED`: the decode process has already completed.

### KS_PARAM_TYPE

**Definition:**

```cpp
typedef enum {
    KS_PARAM_BUFFER_SIZE,
    KS_PARAM_KEY1,
    KS_PARAM_KEY2,
    KS_PARAM_KEY
} KS_PARAM_TYPE;
```

**Description:**

- `KS_PARAM_BUFFER_SIZE`: unused.
- `KS_PARAM_KEY1`: sets the key #1.
- `KS_PARAM_KEY2`: sets the key #2.
- `KS_PARAM_KEY`: sets both key #1 and #2.

### HCA_CIPHER_TYPE

**Definition:**

```cpp
typedef enum {
    HCA_CIPHER_TYPE_NO_CIPHER = 0,
    HCA_CIPHER_TYPE_STATIC = 1,
    HCA_CIPHER_TYPE_WITH_KEY = 0x38
} HCA_CIPHER_TYPE;
```

**Description:**

- `HCA_CIPHER_TYPE_NO_CIPHER`: cipher operation is not necessary.
- `HCA_CIPHER_TYPE_STATIC`: the file uses a globally static key for cipher operation.
- `HCA_CIPHER_TYPE_WITH_KEY`: the file uses two 32-bit keys for cipher operation.

## Primitive Types

**Definition:**

```cpp
typedef signed long long int64;
typedef unsigned long long uint64;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed char int8;
typedef unsigned char uint8;
typedef uint32 ubool;
```
