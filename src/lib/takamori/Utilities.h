#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT Utilities {

    public:

#ifdef __CGSS_OS_WINDOWS__
        typedef HMODULE HLIB;
#endif
#ifdef __CGSS_OS_UNIX__
        typedef void *HLIB;
#endif

        static HLIB LoadDynamicLibrary(LPCSTR lpstrModuleName);

        static void *GetFunctionAddress(HLIB hModule, LPCSTR lpstrFuncName);

        static bool_t FreeDynamicLibrary(HLIB hModule);

    PURE_STATIC(Utilities);

    };

CGSS_NS_END
