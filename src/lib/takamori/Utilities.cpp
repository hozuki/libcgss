#include "Utilities.h"

#if defined(__CGSS_OS_WINDOWS__)

#include <Windows.h>

#elif defined(__CGSS_OS_UNIX__)

#include <dlfcn.h>

#endif

CGSS_NS_BEGIN

    cgss::Utilities::HLIB Utilities::LoadDynamicLibrary(const char *lpstrModuleName) {
#if defined(__CGSS_OS_WINDOWS__)
        return LoadLibrary(lpstrModuleName);
#elif defined(__CGSS_OS_UNIX__)
        return dlopen(lpstrModuleName, RTLD_LAZY);
#endif
    }

    void *Utilities::GetFunctionAddress(cgss::Utilities::HLIB hModule, const char *lpstrFuncName) {
#if defined(__CGSS_OS_WINDOWS__)
        return (void *)GetProcAddress((HMODULE)hModule, lpstrFuncName);
#elif defined(__CGSS_OS_UNIX__)
        return dlsym(hModule, lpstrFuncName);
#endif
    }

    bool_t Utilities::FreeDynamicLibrary(cgss::Utilities::HLIB hModule) {
#if defined(__CGSS_OS_WINDOWS__)
        return static_cast<bool_t>(FreeLibrary((HMODULE)hModule));
#elif defined(__CGSS_OS_UNIX__)
        return static_cast<bool_t>(dlclose(hModule));
#endif
    }

CGSS_NS_END
