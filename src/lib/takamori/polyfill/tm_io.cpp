#include <stdio.h>
#include "../../common/libcgss_building_dll.h"
#include "../../tm_io.h"

CGSS_API_TYPE(TM_HFILE) TmOpenFile(LPCSTR lpcFileName, uint32 dwDesiredAccess, uint32 dwCreationDisposition) {
#ifdef __ON_WINDOWS__
    return CreateFile(lpcFileName, dwDesiredAccess, 0, nullptr, dwCreationDisposition, 0, nullptr);
#else
    char access[4] = {0, 0, 0, 0};
    if (dwDesiredAccess & GENERIC_WRITE) {
        access[0] = 'w';
    } else {
        access[0] = 'r';
    }
    if (dwCreationDisposition == OPEN_ALWAYS || dwCreationDisposition == OPEN_EXISTING) {
        access[0] = 'a';
    }
    access[1] = 'b';
    if ((dwDesiredAccess & GENERIC_WRITE) && (dwDesiredAccess & GENERIC_READ)) {
        access[2] = '+';
    }
    return fopen(lpcFileName, access);
#endif
}

CGSS_API_TYPE(size_t) TmGetFileSize(TM_HFILE hFile) {
#ifdef __ON_WINDOWS__
    DWORD dwFileSize;
    GetFileSize(hFile, &dwFileSize);
    return (long)dwFileSize;
#else
    long position = ftell(hFile);
    fseek(hFile, 0, SEEK_END);
    long p = ftell(hFile);
    fseek(hFile, position, SEEK_SET);
    return (size_t)p;
#endif
}

CGSS_API_TYPE(size_t) TmReadFile(TM_HFILE hFile, uint8 *pBuffer, uint32 dwBufferSize) {
#ifdef __ON_WINDOWS__
    DWORD read;
    auto b = ReadFile(hFile, pBuffer, dwBufferSize, &read, nullptr);
    return b ? (size_t)read : 0;
#else
    auto read = fread(pBuffer, (size_t)dwBufferSize, 1, hFile);
    return read;
#endif
}

CGSS_API_TYPE(size_t) TmWriteFile(TM_HFILE hFile, uint8 *pBuffer, uint32 dwBufferSize) {
#ifdef __ON_WINDOWS__
    DWORD read;
    auto b = WriteFile(hFile, pBuffer, dwBufferSize, &read, nullptr);
    return b ? (size_t)read : 0;
#else
    auto read = fwrite(pBuffer, (size_t)dwBufferSize, 1, hFile);
    return read;
#endif
}

CGSS_API_TYPE(void) TmCloseFile(TM_HFILE hFile) {
#ifdef __ON_WINDOWS__
    CloseHandle(hFile);
#else
    fclose(hFile);
#endif
}
