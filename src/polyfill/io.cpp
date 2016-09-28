#include "io.h"

HANDLE _OpenFile(LPCSTR lpcFileName, DWORD dwDesiredAccess, DWORD dwCreationDisposition) {
#ifdef __CYGWIN__
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

long _GetFileSize(HANDLE hFile) {
#ifdef __CYGWIN__
    LARGE_INTEGER li;
    GetFileSizeEx(hFile, &li);
    return (long)li.LowPart;
#else
    long position = ftell(hFile);
    fseek(hFile, 0, SEEK_END);
    long p = ftell(hFile);
    fseek(hFile, position, SEEK_SET);
    return (size_t)p;
#endif
}

size_t _ReadFile(HANDLE hFile, void *pBuffer, DWORD dwBufferSize) {
#ifdef __CYGWIN__
    DWORD read;
    auto b = ReadFile(hFile, pBuffer, dwBufferSize, &read, nullptr);
    return b ? (size_t)read : 0;
#else
    auto read = fread(pBuffer, (size_t)dwBufferSize, 1, hFile);
    return read;
#endif
}

size_t _WriteFile(HANDLE hFile, void *pBuffer, DWORD dwBufferSize) {
#ifdef __CYGWIN__
    DWORD read;
    auto b = WriteFile(hFile, pBuffer, dwBufferSize, &read, nullptr);
    return b ? (size_t)read : 0;
#else
    auto read = fwrite(pBuffer, (size_t)dwBufferSize, 1, hFile);
    return read;
#endif
}

void _CloseFile(HANDLE hFile) {
#ifdef __CYGWIN__
    CloseHandle(hFile);
#else
    fclose(hFile);
#endif
}
