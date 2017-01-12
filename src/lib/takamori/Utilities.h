#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT Utilities {

    public:

#ifdef __CGSS_OS_UNIX__
        typedef void *HMODULE;
        typdef const char *LPCSTR;
#endif

        static HMODULE LoadDynamicLibrary(LPCSTR lpstrModuleName);

        static void *GetFunctionAddress(HMODULE hModule, LPCSTR lpstrFuncName);

        static bool_t FreeDynamicLibrary(HMODULE hModule);

    PURE_STATIC(Utilities);

    };

CGSS_NS_END
