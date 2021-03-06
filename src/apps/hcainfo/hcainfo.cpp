#include <iostream>
#include "../../lib/cgss_api.h"

using namespace std;

static void PrintAppTitle(std::ostream &out);

static void PrintHelp();

static void PrintInfo(const char *fileName, const HCA_INFO &info);

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        PrintHelp();
        return 0;
    }

    PrintAppTitle(cout);
    bool anyFileFailed = false;

    try {
        for (auto i = 1; i < argc; ++i) {
            const auto fileName = argv[i];

            if (!cgssHelperFileExists(fileName)) {
                anyFileFailed = true;
                cerr << "Error: File '" << fileName << "' does not exist." << endl;
                continue;
            }

            cgss::CFileStream fileStream(fileName, cgss::FileMode::OpenExisting, cgss::FileAccess::Read);
            cgss::CHcaDecoder decoder(&fileStream);

            HCA_INFO hcaInfo;
            decoder.GetHcaInfo(hcaInfo);

            PrintInfo(fileName, hcaInfo);
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

    return anyFileFailed ? -1 : 0;
}

static void PrintAppTitle(std::ostream &out) {
    out << "hcainfo: HCA file info viewer" << endl << endl;
}

static void PrintHelp() {
    PrintAppTitle(cerr);

    cerr << "Usage:" << endl;
    cerr << "  hcainfo <HCA file 1> [<HCA file 2> [...]]" << endl;
}

static void PrintInfo(const char *fileName, const HCA_INFO &info) {
    cout << "---- Information of '" << fileName << "' ----" << endl;
    cout << "General:" << endl;
    cout << "  Version: " << info.versionMajor << "." << info.versionMinor << endl;
    cout << "  Channels: " << info.channelCount << endl;
    cout << "  Sampling rate: " << info.samplingRate << "Hz" << endl;
    cout << "  Data offset (header size): " << info.dataOffset << endl;
    cout << "  Has loop: " << (info.loopExists ? "Yes" : "No") << endl;
    cout << "Decoding:" << endl;
    cout << "  Number of blocks: " << info.blockCount << endl;
    cout << "  Size of each block: " << info.blockSize << endl;
    cout << "  Cipher type: 0x" << hex << info.cipherType << dec << endl;
    switch (info.cipherType) {
        case CGSS_HCA_CIPH_NO_CIPHER:
            cout << "    (Not encrypted)" << endl;
            break;
        case CGSS_HCA_CIPH_STATIC:
            cout << "    (Encrypted with a pre-generated key)" << endl;
            break;
        case CGSS_HCA_CIPH_WITH_KEY:
            cout << "    (Encrypted with a custom key)" << endl;
            break;
        default:
            cout << "    (ERROR)" << endl;
            break;
    }
    if (info.loopExists) {
        cout << "Loop:" << endl;
        cout << "  Start index: " << info.loopStart << endl;
        cout << "  End index: " << info.loopEnd << endl;
    }
    cout << "Other:" << endl;
    cout << "  ATH: " << info.athType << endl;
    cout << "  Volume adjustment: " << info.rvaVolume << endl;
}
