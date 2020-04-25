#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT Utilities {

    public:

        static void *LoadDynamicLibrary(const char *lpstrModuleName);

        static void *GetFunctionAddress(const void *hModule, const char *lpstrFuncName);

        static bool_t FreeDynamicLibrary(void *hModule);

    PURE_STATIC(Utilities);

    };

CGSS_NS_END
