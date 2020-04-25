#include <cstdio>
#include <Windows.h>

#include "../../lib/cgss_api.h"

#define VGAUDIO_APP_LINK "https://github.com/hozuki/vgaudio-cpp"
#define HCAENC_DLL_FILE_NAME "hcaenc_lite.dll"
#define VGAUDIO_APP_VISIT_MESSAGE "For the standalone version (which does not depend on " HCAENC_DLL_FILE_NAME ") please visit <" VGAUDIO_APP_LINK ">."
#define HCAENC_APP_NAME "hcaenc"

#ifdef __CGSS_OS_WINDOWS__

#ifdef __CGSS_ARCH_X86__

#include <iostream>

#include "../../lib/takamori/Utilities.h"

typedef LONG(__stdcall *HCA_ENC_ENCODE_TO_FILE)
    (LPCSTR lpstrInput, LPCSTR lpstrOutput, LONG nQuality, LONG nCutoff, ULONGLONG ullKey);

using namespace std;

static const char *lib_dllName = HCAENC_DLL_FILE_NAME;
static const char *fn_hcaencEncodeToFile = "hcaencEncodeToFile";

HCA_ENC_ENCODE_TO_FILE hcaencEncodeToFile = nullptr;

static void PrintAppTitle(FILE *out);

static void PrintHelp();

int main(int argc, const char *argv[]) {
    cgss::Utilities::HLIB hHcaEncDll = nullptr;
    LPCSTR inputFile = nullptr;
    LPCSTR outputFile = nullptr;
    // These are (nearly) the same settings as High audio profile of CGSS.
    LONG quality = 1;
    LONG cutoff = 0;
    ULONGLONG key = 0ul;

    if (argc < 3) {
        PrintHelp();
        return 0;
    }

    PrintAppTitle(stdout);

    inputFile = argv[1];
    outputFile = argv[2];

    hHcaEncDll = cgss::Utilities::LoadDynamicLibrary(lib_dllName);
    LONG ret = 0;
    if (hHcaEncDll) {
        hcaencEncodeToFile = HCA_ENC_ENCODE_TO_FILE(cgss::Utilities::GetFunctionAddress(hHcaEncDll, fn_hcaencEncodeToFile));
        if (hcaencEncodeToFile) {
            ret = hcaencEncodeToFile(inputFile, outputFile, quality, cutoff, key);
            cout << "Encoder Returns: %d" << (int)ret << endl;
        } else {
            cerr << "ERROR: Failed to locate function " << fn_hcaencEncodeToFile << endl;
        }
        cgss::Utilities::FreeDynamicLibrary(hHcaEncDll);
    } else {
        cerr << "ERROR: Failed to load <" << lib_dllName << ">" << endl;
    }
    return (int)ret;
}

static void PrintAppTitle(FILE *out) {
    static const char *appTitle = HCAENC_APP_NAME ": HCA encoder utility\n\n";

    fprintf(out, "%s", appTitle);
}

static void PrintHelp() {
    PrintAppTitle(stderr);

    static const char *msg_help = "* requires " HCAENC_DLL_FILE_NAME "\n"
                                  VGAUDIO_APP_VISIT_MESSAGE "\n"
                                  "\n"
                                  "Usage:\n"
                                  "  " HCAENC_APP_NAME " <input WAVE> <output HCA>\n\n"
                                  "Example:\n"
                                  "  " HCAENC_APP_NAME " C:\\song_9001.wav C:\\song_9001.hca\n";
    fprintf(stderr, "%s", msg_help);
}

#define _MAIN_DEFINED

#else

#define HCAENC_OBSOLETE_MESSAGE_BASE HCAENC_APP_NAME " is only supported in 32-bit environments because of " HCAENC_DLL_FILE_NAME "."

#endif

#else

#define HCAENC_OBSOLETE_MESSAGE_BASE HCAENC_APP_NAME " is only supported on Windows because of " HCAENC_DLL_FILE_NAME "."

#endif

#ifndef _MAIN_DEFINED

#define HCAENC_OBSOLETE_MESSAGE HCAENC_OBSOLETE_MESSAGE_BASE "\n" VGAUDIO_APP_VISIT_MESSAGE

#pragma message(HCAENC_OBSOLETE_MESSAGE)

int main() {
    static const char *message = HCAENC_OBSOLETE_MESSAGE;
    fprintf(stderr, "%s\n", message);
    return 0;
}

#endif
