#include <stdio.h>
#include "../../lib/tm_api.h"

#ifdef __ARCH_X86__

#include <iostream>

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

    hHcaEncDll = TmLoadLibrary(lib_dllName);
    LONG ret = 0;
    if (hHcaEncDll) {
        hcaencEncodeToFile = HCA_ENC_ENCODE_TO_FILE(TmGetProcAddress(hHcaEncDll, fn_hcaencEncodeToFile));
        if (hcaencEncodeToFile) {
            ret = hcaencEncodeToFile(inputFile, outputFile, quality, cutoff, key);
            cout << "Encoder Returns: %d" << (int)ret << endl;
        } else {
            cerr << "ERROR: Failed to locate function " << fn_hcaencEncodeToFile << endl;
        }
        TmFreeLibrary(hHcaEncDll);
    } else {
        cerr << "ERROR: Failed to load <" << lib_dllName << ">" << endl;
    }
    return (int)ret;
}

#else
#warning hcaenc is only supported in 32-bit environments because of hcaenc_lite.dll.

#include <stdio.h>

int main() {
    printf("hcaenc is only supported in 32-bit environments because of hcaenc_lite.dll.");
    return 0;
}

#endif
