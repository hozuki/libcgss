#include <stdio.h>
#include <iostream>
#include "../../lib/kawashima/ksapi.h"
#include "../../polyfill/polyfill.h"

#define COMPILE_WITH_CGSS

#ifdef COMPILE_WITH_CGSS

#include "../cgssh.h"

#endif

using namespace std;

void PrintHelp();

uint32 hatoui(const char *a);

int main(int argc, const char *argv[]) {
    int argc0;
    char **argv0;

    // Blame Cygwin for the WTH command line parsing on Windows, that I have to write this hack!
#ifdef __CYGWIN__
    LPWSTR commandline = GetCommandLineW();
    LPWSTR *args = CommandLineToArgvW(commandline, &argc0);
    argv0 = new char *[argc0];
    for (auto i = 0; i < argc0; ++i) {
        auto num = WideCharToMultiByte(CP_OEMCP, 0, args[i], -1, nullptr, 0, nullptr, FALSE);
        argv0[i] = new char[num];
        if (!argv0[i]) {
            delete[] argv0[i];
        }
        WideCharToMultiByte(CP_OEMCP, 0, args[i], -1, argv0[i], num, nullptr, FALSE);
    }
#else
    argc0 = argc;
    argv0 = const_cast<char **>(argv);
#endif

    if (argc0 < 3 || argc0 > 5) {
        PrintHelp();
        return 0;
    }

    HKDECODE hDecode = nullptr;
    KS_RESULT result = KS_ERR_OK;
    uint8 *buffer = nullptr;
    uint32 bufferSize = 0;
    uint32 dataSize = 0;
    HANDLE fp = nullptr;

    // Open the file and get a handle.
    result = KsOpenFile(argv0[1], &hDecode);
    if (!KS_CALL_SUCCESSFUL(result)) {
        cout << "Loading file '" << argv0[1] << "' failed." << endl;
        return 1;
    }

    // Set parameters before beginning to decode.
    uint32 key1 = 0, key2 = 0;
#ifdef COMPILE_WITH_CGSS
    key1 = cgssKey1;
    key2 = cgssKey2;
#endif
    if (argc0 >= 4) {
        key1 = hatoui(argv0[3]);
    }
    if (argc0 >= 5) {
        key1 = hatoui(argv0[4]);
    }
    if (argc0 >= 5) {
        KsSetParamI32(hDecode, KS_PARAM_KEY1, key1);
        KsSetParamI32(hDecode, KS_PARAM_KEY2, key2);
    }
    KsBeginDecode(hDecode);
    fp = _OpenFile(argv0[2], GENERIC_WRITE, CREATE_ALWAYS);

    // Write the WAVE header.
    KsGetWaveHeader(hDecode, nullptr, &bufferSize);
    buffer = new uint8[bufferSize];
    KsGetWaveHeader(hDecode, buffer, &bufferSize);
    _WriteFile(fp, buffer, bufferSize);
    delete[] buffer;

    // Write WAVE data blocks.
    KsDecodeData(hDecode, nullptr, &bufferSize);
    if (bufferSize > 0) {
        result = KS_OP_HAS_MORE_DATA;
        while (result > 0) {
            // 10 blocks per time
            dataSize = bufferSize * 10;
            buffer = new uint8[dataSize];
            result = KsDecodeData(hDecode, buffer, &dataSize);
            _WriteFile(fp, buffer, dataSize);
            delete[] buffer;
        }
    }

    // Clean up.
    _CloseFile(fp);
    KsEndDecode(hDecode);
    KsCloseHandle(hDecode);

#ifdef __CYGWIN__
    delete[] argv0;
    // What about `args`?
#endif
}

void PrintHelp() {
    uint32 k1 = 0, k2 = 0;
#ifdef COMPILE_WITH_CGSS
    k1 = cgssKey1;
    k2 = cgssKey2;
#endif
    cout << "Usage:\r\nhca2wav <in file> <out file> "
         << "[<key1 = " << hex << k1 << "> <key2 = " << hex << k2 << ">]"
         << endl;
}

uint32 hatoui(const char *a) {
    uint32 val = 0;
    auto i = 0;
    while (a && *a && i < (sizeof(uint32) / sizeof(uint8) * 2)) {
        if ('0' <= *a && *a <= '9') {
            val = (val << 4) + (*a - '0');
        } else if ('a' <= *a && *a <= 'f') {
            val = (val << 4) + (*a - 'a' + 10);
        } else if ('A' <= *a && *a <= 'F') {
            val = (val << 4) + (*a - 'A' + 10);
        } else {
            break;
        }
        ++i;
        ++a;
    }
    return val;
}
