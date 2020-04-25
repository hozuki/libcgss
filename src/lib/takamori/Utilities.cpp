#include "../cgss_env.h"

#ifdef __CGSS_OS_WINDOWS__

#include <Windows.h>

#elif __CGSS_OS_UNIX__
#include <dlfcn.h>
#endif

#include "Utilities.h"

CGSS_NS_BEGIN

    void *Utilities::LoadDynamicLibrary(const char *lpstrModuleName) {
#if defined(__CGSS_OS_WINDOWS__)
        return LoadLibrary(lpstrModuleName);
#elif defined(__CGSS_OS_UNIX__)
        return dlopen(lpstrModuleName, RTLD_LAZY);
#endif
    }

    void *Utilities::GetFunctionAddress(const void *hModule, const char *lpstrFuncName) {
#if defined(__CGSS_OS_WINDOWS__)
        return (void *)GetProcAddress((HMODULE)hModule, lpstrFuncName);
#elif defined(__CGSS_OS_UNIX__)
        return dlsym(hModule, lpstrFuncName);
#endif
    }

    bool_t Utilities::FreeDynamicLibrary(void *hModule) {
#if defined(__CGSS_OS_WINDOWS__)
        return static_cast<bool_t>(FreeLibrary((HMODULE)hModule));
#elif defined(__CGSS_OS_UNIX__)
        return static_cast<bool_t>(dlclose(hModule));
#endif
    }

CGSS_NS_END
