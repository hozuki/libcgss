#ifndef CGSS_POLYFILL_H
#define CGSS_POLYFILL_H

#if (defined(_WIN64) || defined(__LP64__) || defined(__LLP64__))
#define __ARCH_X64__
#else
#define __ARCH_X86__
#endif

#if (defined(_WIN32) || defined(__CYGWIN__))

#define __ON_WINDOWS__
#ifndef _MBCS
#define _MBCS
#endif
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#else

#define __ON_UNIX__

#endif

#ifdef __ON_WINDOWS__
typedef HANDLE TM_HFILE;
typedef HMODULE TM_HMODULE;
#endif
#ifdef __ON_UNIX__

#include <stdio.h>

typedef FILE *TM_HFILE;
typedef const char *LPCSTR;
#define GENERIC_READ (0x80000000)
#define GENERIC_WRITE (0x40000000)
#define GENERIC_ALL (0xc0000000)
#define CREATE_NEW (1)
#define CREATE_ALWAYS (2)
#define OPEN_EXISTING (3)
#define OPEN_ALWAYS (4)
#define TRUNCATE_EXISTING (5)

#include <dlfcn.h>

typedef void *TM_HMODULE;
#endif

#include "cgss_typedef.h"
#include "tm_dylib.h"
#include "tm_io.h"

#endif //CGSS_POLYFILL_H
