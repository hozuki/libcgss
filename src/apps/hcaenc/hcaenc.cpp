#if (defined(_WIN64) || defined(__LP64__) || defined(__LLP64__))

#ifndef _MBCS
#define _MBCS
#endif

#include <iostream>

#if (defined(_WIN32) || defined(__CYGWIN__))
#define __ON_WINDOWS__

#include "../../win_header.h"
#include <windows.h>

#else
#define __ON_UNIX__
#include <dlfcn.h>
typedef signed long LONG;
typedef const char *LPCSTR;
typedef unsigned long long ULONGLONG;
typedef void *HMODULE;
typedef unsigned int BOOL;
#endif

#ifndef __ON_WINDOWS__
#error Currently hcaenc only supports Windows builds because of hcaenc_lite.dll.
#endif

typedef LONG(__stdcall *HCA_ENC_ENCODE_TO_FILE)
        (LPCSTR lpstrInput, LPCSTR lpstrOutput, LONG nQuality, LONG nCutoff, ULONGLONG ullKey);

using namespace std;

static const char *lib_dllName = "hcaenc_lite.dll";
static const char *fn_hcaencEncodeToFile = "hcaencEncodeToFile";

HCA_ENC_ENCODE_TO_FILE hcaencEncodeToFile = nullptr;

static const char *msg_help = ""
        "HCA Encoder Utility\n\n"
        "Usage:\n"
        "  hcaenc.exe <input WAVE> <output HCA>\n\n"
        "Example:\n"
        "  hcaenc.exe C:\\song_9001.wav C:\\song_9001.hca";

inline HMODULE _LoadLibrary(LPCSTR moduleName) {
#if defined(__ON_WINDOWS__)
    return LoadLibrary(moduleName);
#elif defined(__ON_UNIX__)
    return dlopen(moduleName, RTLD_LAZY);
#endif
}

inline void *_GetProcAddress(HMODULE module, LPCSTR functionName) {
#if defined(__ON_WINDOWS__)
    return (void *)GetProcAddress(module, functionName);
#elif defined(__ON_UNIX__)
    return dlsym(module, functionName);
#endif
}

inline BOOL _FreeLibrary(HMODULE module) {
#if defined(__ON_WINDOWS__)
    return FreeLibrary(module);
#elif defined(__ON_UNIX__)
    return (BOOL)dlclose(module);
#endif
}

int main(int argc, const char *argv[]) {
    HMODULE hHcaEncDll = nullptr;
    LPCSTR inputFile = nullptr;
    LPCSTR outputFile = nullptr;
    // These are (nearly) the same settings as High audio profile of CGSS.
    LONG quality = 1;
    LONG cutoff = 0;
    ULONGLONG key = 0ul;

    if (argc < 3) {
        cout << msg_help << endl;
        return 0;
    } else {
        inputFile = argv[1];
        outputFile = argv[2];
    }

    hHcaEncDll = _LoadLibrary(lib_dllName);
    LONG ret = 0;
    if (hHcaEncDll) {
        hcaencEncodeToFile = HCA_ENC_ENCODE_TO_FILE(_GetProcAddress(hHcaEncDll, fn_hcaencEncodeToFile));
        if (hcaencEncodeToFile) {
            ret = hcaencEncodeToFile(inputFile, outputFile, quality, cutoff, key);
            cout << "Encoder Returns: %d" << (int)ret << endl;
        } else {
            cerr << "ERROR: Failed to locate function " << fn_hcaencEncodeToFile << endl;
        }
        _FreeLibrary(hHcaEncDll);
    } else {
        cerr << "ERROR: Failed to load <" << lib_dllName << ">" << endl;
    }
    return (int)ret;
}

#else
#warning hcaenc is only supported in 32-bit environments because of hcaenc_lite.dll.
#endif
