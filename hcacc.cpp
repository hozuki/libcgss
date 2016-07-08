#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "hca/hca_info.h"
#include "hca/CHCA.h"

using namespace std;

#define NOT_COMPILE_WITH_CGSS
#ifdef COMPILE_WITH_CGSS

#include "cgssh.h"

#endif

static const char *msg_help = ""
        "HCA Cipher Conversion Utility\n\n"
        "Usage:\n"
        "  hcacc.exe <input HCA> <output HCA> "
        "[-ot <output HCA cipher type>] "
        "[-i1 <input HCA key 1 (if any)>] [-i2 <input HCA key 2 (if any)>] "
        "[-o1 <output HCA key 1>] [-o2 <output HCA key 2>]\n\n"
        "Remarks:\n"
        "  - Valid crypto types are: 0, 1, 56.\n"
        "  - Keys are entered in 4 byte hex form, e.g.: 0403F18B.\n"
        "  - Default value of all arguments is 0.\n"
        "Example:\n"
        "  hcacc.exe C:\\in.hca C:\\out.hca -ot 1\n"
        "  * This command will convert an HCA file from cipher type 0 (no cipher) to type 1 (with static cipher key).";

int parseArgs(int argc, const char *argv[], const char **input, const char **output,
              HCA_CIPHER_CONFIG &ccFrom, HCA_CIPHER_CONFIG &ccTo);

uint32 atoh(const char *str);

uint32 atoh(const char *str, int max_length);

int main(int argc, const char *argv[]) {
    //const char *fileFrom = "D:\\Documents\\CriAtomCraft\\song_1004\\Public\\song_1004\\_deretore_acb_extract_song_1004.acb\\acb\\awb\\song_1004.hca";//"D:\\Documents\\CriAtomCraft\\CgssCrackTest\\Public\\_deretore_acb_extract_song_1004.acb\\acb\\awb\\song_1004.hca";
    //const char *fileFrom = "img.hca";
    //const char *fileFrom = "D:\\Documents\\CriAtomCraft\\song_1004\\Public\\song_1004\\_deretore_acb_extract_song_1004.acb\\acb\\awb\\song_1004-orig.hca";
    //const char *fileTo = "D:\\Documents\\CriAtomCraft\\song_1004\\Public\\song_1004\\_deretore_acb_extract_song_1004.acb\\acb\\awb\\song_1004a.hca";
    HCA_CIPHER_CONFIG ccFrom, ccTo;
    FILE *fp;
    long fileSize;
    uint8 *buf = nullptr;
    CHCA *hca;
    const char *fileFrom, *fileTo;

    memset(&ccFrom, 0, sizeof(HCA_CIPHER_CONFIG));
    memset(&ccTo, 0, sizeof(HCA_CIPHER_CONFIG));

    int r = parseArgs(argc, argv, &fileFrom, &fileTo, ccFrom, ccTo);
    if (r > 0) {
        // An error occurred.
        cerr << "Argument error: " << r << endl;
        return r;
    } else if (r < 0) {
        // Help message is printed.
        return 0;
    }

#ifdef COMPILE_WITH_CGSS
    ccFrom.keyParts.key1 = cgssKey1;
    ccFrom.keyParts.key2 = cgssKey2;
    ccTo.cipherType = HCA_CIPHER_TYPE_WITH_KEY;
    ccTo.keyParts.key1 = cgssKey1;
    ccTo.keyParts.key2 = cgssKey2;
#endif

    fp = fopen(fileFrom, "rb");
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    buf = (uint8 *)malloc(fileSize * sizeof(uint8));
    fseek(fp, 0, SEEK_SET);
    fread(buf, (size_t)fileSize, 1, fp);
    fclose(fp);

    hca = new CHCA(ccFrom, ccTo);

    uint32 dataOffset = 0;
    KS_RESULT result = KS_ERR_OK;
    if (KS_CALL_SUCCESSFUL(result)) {
        result = hca->ReadHeader(buf, (uint32)fileSize, &dataOffset);
    }
    if (KS_CALL_SUCCESSFUL(result)) {
        hca->ConvertData(buf, (uint32)fileSize, dataOffset);
    }
    if (KS_CALL_SUCCESSFUL(result)) {
        hca->SetNewCipherType(buf, (uint32)fileSize, ccTo.cipherType, dataOffset);
    }

    if (KS_CALL_SUCCESSFUL(result)) {
        fp = fopen(fileTo, "wb");
        fwrite(buf, (size_t)fileSize, 1, fp);
        fclose(fp);
    }

    delete hca;
    free(buf);

    return result;
}

#define SWITCH_HASH(char1, char2) (uint32)(((uint32)(char1) << 8) | (uint32)(char2))

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
            uint32 switchHash = SWITCH_HASH(argv[i][1], argv[i][2]);
            switch (switchHash) {
                case SWITCH_HASH('o', 't'):
                    if (i + 1 < argc) {
                        int cipherType = atoi(argv[++i]);
                        if (cipherType != 0 && cipherType != 1 && cipherType != 0x38) {
                            return 1;
                        }
                        ccTo.cipherType = (HCA_CIPHER_TYPE)cipherType;
                    }
                    break;
                case SWITCH_HASH('i', '1'):
                    if (i + 1 < argc) {
                        ccFrom.keyParts.key1 = atoh(argv[++i]);
                    }
                    break;
                case SWITCH_HASH('i', '2'):
                    if (i + 1 < argc) {
                        ccFrom.keyParts.key2 = atoh(argv[++i]);
                    }
                    break;
                case SWITCH_HASH('o', '1'):
                    if (i + 1 < argc) {
                        ccTo.keyParts.key1 = atoh(argv[++i]);
                    }
                    break;
                case SWITCH_HASH('o', '2'):
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

uint32 atoh(const char *str) {
    return atoh(str, 8);
}

uint32 atoh(const char *str, int max_length) {
    max_length = min(max_length, 8);
    int i = 0;
    uint32 ret = 0;
    while (i < max_length && *str) {
        if (IS_NUM(*str)) {
            ret = (ret << 4) | (uint32)(*str - '0');
        } else if (IS_UPHEX(*str)) {
            ret = (ret << 4) | (uint32)(*str - 'A' + 10);
        } else if (IS_LOHEX(*str)) {
            ret = (ret << 4) | (uint32)(*str - 'a' + 10);
        } else {
            break;
        }
        ++str;
    }
    return ret;
}
