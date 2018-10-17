#include <iostream>
#include "../../lib/cgss_api.h"
#include "../cgssh.h"

using namespace std;

void PrintHelp();

template<typename T>
T hex_alpha_to_integer(const char *a);

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

    if (!(argc0 == 3 || argc0 == 5)) {
        PrintHelp();
        return 0;
    }

    // Configure decoder.
    cgss::CHcaDecoderConfig decoderConfig;
    decoderConfig.decodeFunc = cgss::CDefaultWaveGenerator::Decode16BitS;
    decoderConfig.waveHeaderEnabled = TRUE;

    auto &pKey1 = decoderConfig.cipherConfig.keyParts.key1;
    auto &pKey2 = decoderConfig.cipherConfig.keyParts.key2;

    pKey1 = g_CgssKey1;
    pKey2 = g_CgssKey2;

    if (argc0 >= 5) {
        pKey1 = hex_alpha_to_integer<uint32_t>(argv0[3]);
        pKey2 = hex_alpha_to_integer<uint32_t>(argv0[4]);
    }

    if (argc0 >= 6) {
        decoderConfig.cipherConfig.keyModifier = hex_alpha_to_integer<uint16_t>(argv0[5]);
    }

    // Go!
    try {
        cgss::CFileStream fileIn(argv0[1], cgss::FileMode::OpenExisting, cgss::FileAccess::Read),
            fileOut(argv0[2], cgss::FileMode::Create, cgss::FileAccess::Write);
        cgss::CHcaDecoder hcaDecoder(&fileIn, decoderConfig);

        uint32_t read = 1;
        static const uint32_t bufferSize = 1024;
        uint8_t buffer[bufferSize];

        while (read > 0) {
            read = hcaDecoder.Read(buffer, bufferSize, 0, bufferSize);

            if (read > 0) {
                fileOut.Write(buffer, bufferSize, 0, read);
            }
        }
    } catch (const cgss::CException &ex) {
        cerr << "CException: " << ex.GetExceptionMessage() << ", code=" << ex.GetOpResult() << endl;
        return ex.GetOpResult();
    } catch (const std::logic_error &ex) {
        cerr << "std::logic_error: " << ex.what() << endl;
        return 1;
    } catch (const std::runtime_error &ex) {
        cerr << "std::runtime_error: " << ex.what() << endl;
        return 1;
    }

    cout << "Completed." << endl;

#ifdef __CYGWIN__
    delete[] argv0;
    // What about `args`?
#endif
    return 0;
}

void PrintHelp() {
    uint32_t k1 = 0, k2 = 0;
    k1 = g_CgssKey1;
    k2 = g_CgssKey2;
    cout << "hca2wav: Utility for decoding HCA to wave audio\n\n"
         << "Usage:\n"
         << "    hca2wav <in file> <out file> [<decode key 1 = " << hex << k1 << "> <decode key 2 = " << hex << k2 << ">] [<keymod>]\n"
         << "Example:\n"
         << "    hca2wav C:\\input.hca C:\\output.wav 12345678 90abcdef a1b2"
         << endl;
}

template<typename T>
T hex_alpha_to_integer(const char *a) {
    T val = 0;
    auto i = 0;

    while (a && *a && i < (sizeof(T) / sizeof(uint8_t) * 2)) {
        if ('0' <= *a && *a <= '9') {
            val = (val << 4u) + (*a - '0');
        } else if ('a' <= *a && *a <= 'f') {
            val = (val << 4u) + (*a - 'a' + 10);
        } else if ('A' <= *a && *a <= 'F') {
            val = (val << 4u) + (*a - 'A' + 10);
        } else {
            break;
        }

        ++i;
        ++a;
    }

    return val;
}
