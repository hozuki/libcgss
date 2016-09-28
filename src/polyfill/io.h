#ifndef CGSS_IO_H
#define CGSS_IO_H

#include "polyfill.h"
#include <stdlib.h>

#ifdef __ON_UNIX__

#include <cstdio>

typedef FILE *HANDLE;
typedef const char *LPCSTR;
typedef unsigned long DWORD;
typedef DWORD BOOL;
#define GENERIC_READ (0x80000000)
#define GENERIC_WRITE (0x40000000)
#define GENERIC_ALL (0xc0000000)
#define CREATE_NEW (1)
#define CREATE_ALWAYS (2)
#define OPEN_EXISTING (3)
#define OPEN_ALWAYS (4)
#define TRUNCATE_EXISTING (5)
#endif

HANDLE _OpenFile(LPCSTR lpcFileName, DWORD dwDesiredAccess, DWORD dwCreationDisposition);

long _GetFileSize(HANDLE hFile);

size_t _ReadFile(HANDLE hFile, void *pBuffer, DWORD dwBufferSize);

size_t _WriteFile(HANDLE hFile, void *pBuffer, DWORD dwBufferSize);

void _CloseFile(HANDLE hFile);

#endif //CGSS_IO_H
