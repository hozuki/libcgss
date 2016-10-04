#include "../../common/cgss_building_dll.h"
#include "../../tm_dylib.h"

CGSS_API_TYPE(TM_HMODULE) TmLoadLibrary(LPCSTR lpstrModuleName) {
#if defined(__ON_WINDOWS__)
    return LoadLibrary(lpstrModuleName);
#elif defined(__ON_UNIX__)
    return dlopen(lpstrModuleName, RTLD_LAZY);
#endif
}

CGSS_API_TYPE(void *)TmGetProcAddress(TM_HMODULE hModule, LPCSTR lpstrFuncName) {
#if defined(__ON_WINDOWS__)
    return (void *)GetProcAddress(hModule, lpstrFuncName);
#elif defined(__ON_UNIX__)
    return dlsym(hModule, lpstrFuncName);
#endif
}

CGSS_API_TYPE(ubool) TmFreeLibrary(TM_HMODULE hModule) {
#if defined(__ON_WINDOWS__)
    return (ubool)FreeLibrary(hModule);
#elif defined(__ON_UNIX__)
    return (ubool)dlclose(hModule);
#endif
}