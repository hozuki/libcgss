#include "dl.h"

HMODULE _LoadLibrary(LPCSTR moduleName) {
#if defined(__ON_WINDOWS__)
    return LoadLibrary(moduleName);
#elif defined(__ON_UNIX__)
    return dlopen(moduleName, RTLD_LAZY);
#endif
}

void *_GetProcAddress(HMODULE module, LPCSTR functionName) {
#if defined(__ON_WINDOWS__)
    return (void *)GetProcAddress(module, functionName);
#elif defined(__ON_UNIX__)
    return dlsym(module, functionName);
#endif
}

BOOL _FreeLibrary(HMODULE module) {
#if defined(__ON_WINDOWS__)
    return FreeLibrary(module);
#elif defined(__ON_UNIX__)
    return (BOOL)dlclose(module);
#endif
}