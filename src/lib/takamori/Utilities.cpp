#include "Utilities.h"

#ifdef __CGSS_OS_UNIX__
#include <dlfcn.h>
#endif

CGSS_NS_BEGIN

    Utilities::HLIB Utilities::LoadDynamicLibrary(LPCSTR lpstrModuleName) {
#if defined(__CGSS_OS_WINDOWS__)
        return LoadLibrary(lpstrModuleName);
#elif defined(__CGSS_OS_UNIX__)
        return dlopen(lpstrModuleName, RTLD_LAZY);
#endif
    }

    void *Utilities::GetFunctionAddress(Utilities::HLIB hModule, LPCSTR lpstrFuncName) {
#if defined(__CGSS_OS_WINDOWS__)
        return (void *)GetProcAddress(hModule, lpstrFuncName);
#elif defined(__CGSS_OS_UNIX__)
        return dlsym(hModule, lpstrFuncName);
#endif
    }

    bool_t Utilities::FreeDynamicLibrary(Utilities::HLIB hModule) {
#if defined(__CGSS_OS_WINDOWS__)
        return static_cast<bool_t>(FreeLibrary(hModule));
#elif defined(__CGSS_OS_UNIX__)
        return static_cast<bool_t>(dlclose(hModule));
#endif
    }

CGSS_NS_END
