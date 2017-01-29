#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#ifndef __MINGW_H

#include <algorithm>

#endif

#include "../../lib/cgss_api.h"

using namespace std;

#include "../cgssh.h"

static const char *msg_help = ""
    "hcacc: HCA Cipher Conversion Utility\n\n"
    "Usage:\n"
    "  hcacc.exe <input HCA> <output HCA> [extra options]\n\n"
    "Extra options:\n"
    "  -ot <output HCA cipher type>\n"
    "  -i1 <input HCA key 1 (if necessary)>\n"
    "  -i2 <input HCA key 2 (if necessary)>\n"
    "  -o1 <output HCA key 1>\n"
    "  -o2 <output HCA key 2>\n\n"
    "Remarks:\n"
    "  - Valid cipher types are: 0, 1, 56.\n"
    "  - Keys are entered in 4 byte hex form, e.g.: 0403F18B.\n"
    "  - Default value of all arguments is 0, unless " cgss_str(__COMPILE_WITH_CGSS_KEYS) " is set during compilation.\n\n"
    "Example:\n"
    "  hcacc.exe C:\\in.hca C:\\out.hca -ot 1\n"
    "  * This command will convert an HCA file from cipher type 0 (no cipher) to type 1 (with static cipher key).";

int parseArgs(int argc, const char *argv[], const char **input, const char **output,
              HCA_CIPHER_CONFIG &ccFrom, HCA_CIPHER_CONFIG &ccTo);

uint32_t atoh(const char *str);

uint32_t atoh(const char *str, int max_length);

int main(int argc, const char *argv[]) {
    cgss::CHcaCipherConfig ccFrom, ccTo;
    const char *fileNameFrom, *fileNameTo;

    memset(&ccFrom, 0, sizeof(HCA_CIPHER_CONFIG));
    memset(&ccTo, 0, sizeof(HCA_CIPHER_CONFIG));

    ccFrom.keyParts.key1 = g_CgssKey1;
    ccFrom.keyParts.key2 = g_CgssKey2;
    ccTo.cipherType = CGSS_HCA_CIPH_WITH_KEY;
    ccTo.keyParts.key1 = g_CgssKey1;
    ccTo.keyParts.key2 = g_CgssKey2;

    int r = parseArgs(argc, argv, &fileNameFrom, &fileNameTo, ccFrom, ccTo);
    if (r > 0) {
        // An error occurred.
        cerr << "Argument error: " << r << endl;
        return r;
    } else if (r < 0) {
        // Help message is printed.
        return 0;
    }

    try {
        cgss::CFileStream fileFrom(fileNameFrom, cgss::FileMode::OpenExisting, cgss::FileAccess::Read),
            fileTo(fileNameTo, cgss::FileMode::Create, cgss::FileAccess::Write);
        cgss::CHcaCipherConverter cipherConverter(&fileFrom, ccFrom, ccTo);
        const uint32_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        uint32_t read = 1;
        while (read > 0) {
            read = cipherConverter.Read(buffer, bufferSize, 0, bufferSize);
            if (read > 0) {
                fileTo.Write(buffer, bufferSize, 0, read);
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

    return 0;
}

#define CASE_HASH(char1, char2) (uint32_t)(((uint32_t)(char1) << 8) | (uint32_t)(char2))

int parseArgs(int argc, const char *argv[], const char **input, const char **output, HCA_CIPHER_CONFIG &ccFrom,
              HCA_CIPHER_CONFIG &ccTo) {
    if (argc < 3) {
        cout << msg_help << endl;
        return -1;
    }
    *input = argv[1];
    *output = argv[2];

    for (int i = 3; i < argc; ++i) {
        if (argv[i][0] == '-' || argv[i][0] == '/') {
            uint32_t switchHash = CASE_HASH(argv[i][1], argv[i][2]);
            switch (switchHash) {
                case CASE_HASH('o', 't'):
                    if (i + 1 < argc) {
                        int cipherType = atoi(argv[++i]);
                        if (cipherType != 0 && cipherType != 1 && cipherType != 0x38) {
                            return 1;
                        }
                        ccTo.cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(cipherType);
                    }
                    break;
                case CASE_HASH('i', '1'):
                    if (i + 1 < argc) {
                        ccFrom.keyParts.key1 = atoh(argv[++i]);
                    }
                    break;
                case CASE_HASH('i', '2'):
                    if (i + 1 < argc) {
                        ccFrom.keyParts.key2 = atoh(argv[++i]);
                    }
                    break;
                case CASE_HASH('o', '1'):
                    if (i + 1 < argc) {
                        ccTo.keyParts.key1 = atoh(argv[++i]);
                    }
                    break;
                case CASE_HASH('o', '2'):
                    if (i + 1 < argc) {
                        ccTo.keyParts.key2 = atoh(argv[++i]);
                    }
                    break;
                default:
                    return 2;
            }
        }
    }
    return 0;
}

#define IS_NUM(ch) ('0' <= (ch) && (ch) <= '9')
#define IS_UPHEX(ch) ('A' <= (ch) && (ch) <= 'F')
#define IS_LOHEX(ch) ('a' <= (ch) && (ch) <= 'f')

uint32_t atoh(const char *str) {
    return atoh(str, 8);
}

uint32_t atoh(const char *str, int max_length) {
    max_length = min(max_length, 8);
    int i = 0;
    uint32_t ret = 0;
    while (i < max_length && *str) {
        if (IS_NUM(*str)) {
            ret = (ret << 4) | (uint32_t)(*str - '0');
        } else if (IS_UPHEX(*str)) {
            ret = (ret << 4) | (uint32_t)(*str - 'A' + 10);
        } else if (IS_LOHEX(*str)) {
            ret = (ret << 4) | (uint32_t)(*str - 'a' + 10);
        } else {
            break;
        }
        ++str;
    }
    return ret;
}
