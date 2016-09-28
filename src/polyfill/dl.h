#ifndef CGSS_DL_H
#define CGSS_DL_H

#include "polyfill.h"

#ifdef __ON_UNIX__
#include <dlfcn.h>

typedef signed long LONG;
typedef const char *LPCSTR;
typedef unsigned long long ULONGLONG;
typedef void *HMODULE;
typedef unsigned int BOOL;
#endif

#endif //CGSS_DL_H

HMODULE _LoadLibrary(LPCSTR moduleName);

void *_GetProcAddress(HMODULE module, LPCSTR functionName);

BOOL _FreeLibrary(HMODULE module);
