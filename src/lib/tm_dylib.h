#ifndef CGSS_DL_H
#define CGSS_DL_H

#include "tm_api.h"

DECL_CGSS_API(TM_HMODULE, TmLoadLibrary, (LPCSTR lpstrModuleName));
DECL_CGSS_API(void *, TmGetProcAddress, (TM_HMODULE hModule, LPCSTR lpstrFuncName));
DECL_CGSS_API(ubool, TmFreeLibrary, (TM_HMODULE hModule));

#endif //CGSS_DL_H
