#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT Utilities {

    public:

        typedef void *HLIB;

        static HLIB LoadDynamicLibrary(const char *lpstrModuleName);

        static void *GetFunctionAddress(HLIB hModule, const char *lpstrFuncName);

        static bool_t FreeDynamicLibrary(HLIB hModule);

    PURE_STATIC(Utilities);

    };

CGSS_NS_END
