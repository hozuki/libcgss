#include <iostream>
#include "CHcaFormatReader.h"
#include "hca_native.h"
#include "../../takamori/exceptions/CException.h"
#include "../../takamori/exceptions/CFormatException.h"
#include "hca_utils.h"
#include "../../common/quick_utils.h"
#include "../../takamori/streams/CBinaryReader.h"
#include "../../takamori/exceptions/CInvalidOperationException.h"

CGSS_NS_BEGIN

    class NullHcaReader final : public CHcaFormatReader {

    __extends(CHcaFormatReader, NullHcaReader);

    public:

        explicit NullHcaReader(IStream *baseStream)
            : MyBase(baseStream) {
        }

    private:

        uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override {
            return 0;
        }

        uint64_t GetPosition() override {
            return 0;
        }

        void SetPosition(uint64_t value) override {
        }

        uint64_t GetLength() override {
            return 0;
        }

    };

    CHcaFormatReader::CHcaFormatReader(IStream *baseStream)
        : _baseStream(baseStream) {
        memset(&_hcaInfo, 0, sizeof(HCA_INFO));
        Initialize();
    }

    const uint16_t *CHcaFormatReader::ChecksumTable = new uint16_t[256] {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202,
    };

    uint16_t CHcaFormatReader::ComputeChecksum(void *pData, uint32_t dwDataSize, uint16_t wInitSum) {
        uint8_t *p = (uint8_t *)pData;
        for (uint32_t i = 0; i < dwDataSize; ++i, ++p) {
            wInitSum = (wInitSum << 8) ^ ChecksumTable[(wInitSum >> 8) ^ *p];
        }
        return wInitSum;
    }

    const HCA_INFO &CHcaFormatReader::GetHcaInfo() const {
        return _hcaInfo;
    }

    void CHcaFormatReader::GetHcaInfo(HCA_INFO &info) const {
        memcpy(&info, &_hcaInfo, sizeof(HCA_INFO));
    }

    void CHcaFormatReader::GetHcaInfo(HCA_INFO *pInfo) const {
        if (!pInfo) {
            return;
        }
        memcpy(pInfo, &_hcaInfo, sizeof(HCA_INFO));
    }

    void CHcaFormatReader::Initialize() {
        auto stream = _baseStream;
        auto &hcaInfo = _hcaInfo;
        uint32_t bufferSize;
        uint32_t actualRead;

#define ENSURE_READ_ALL(var) \
        bufferSize = sizeof(var); \
        actualRead = stream->Read(&var, bufferSize, 0, bufferSize); \
        do { \
            if (actualRead < bufferSize) { \
                throw CFormatException("Unexpected end of file."); \
            } \
        } while (0)
#define ENSURE_READ_ALL_BUFFER(buffer, size) \
        bufferSize = size; \
        actualRead = stream->Read(buffer, bufferSize, 0, bufferSize); \
        do { \
            if (actualRead < bufferSize) { \
                throw CFormatException("Unexpected end of file."); \
            } \
        } while (0)

        {
            // Check HCA header (of the whole file).
            HCA_FILE_HEADER hcaFileHeader;
            ENSURE_READ_ALL(hcaFileHeader);
            ensureMagicMatch(hcaFileHeader.hca, Magic::HCA);
            // Calculate the correct data offset.
            // Headers will take up to dataOffset bytes, and after that, audio data.
            uint32_t dataOffset = bswap(hcaFileHeader.dataOffset);
            uint16_t fileVersion = bswap(hcaFileHeader.version);
            hcaInfo.versionMajor = (uint16_t)(fileVersion >> 8);
            hcaInfo.versionMinor = (uint16_t)(fileVersion & 0xff);
            hcaInfo.dataOffset = dataOffset;

            // Read the whole headers section and verify checksum.
            uint8_t *headerContents = new uint8_t[dataOffset];
            stream->Seek(0, StreamSeekOrigin::Begin);
            ENSURE_READ_ALL_BUFFER(headerContents, dataOffset);
            const auto headerChecksum = ComputeChecksum(headerContents, dataOffset, 0);
            if (headerChecksum != 0) {
                throw CException(CGSS_OP_CHECKSUM_ERROR, "Header is corrupted.");
            }
            delete[] headerContents;

            // Go back to next header (FMT).
            stream->Seek(sizeof(HCA_FILE_HEADER), StreamSeekOrigin::Begin);
        }
        // FMT
        {
            HCA_FORMAT_HEADER hcaFormatHeader;
            ENSURE_READ_ALL(hcaFormatHeader);
            ensureMagicMatch(hcaFormatHeader.fmt, Magic::FORMAT);
            hcaInfo.channelCount = hcaFormatHeader.channelCount;
            hcaInfo.samplingRate = bswap(hcaFormatHeader.samplingRate << 8);
            hcaInfo.blockCount = bswap(hcaFormatHeader.blockCount);
            hcaInfo.fmtR01 = bswap(hcaFormatHeader.r01);
            hcaInfo.fmtR02 = bswap(hcaFormatHeader.r02);
            if (!(1 <= hcaInfo.channelCount && hcaInfo.channelCount <= 16)) {
                throw CFormatException("Number of channels is out of range.");
            }
            if (!(1 <= hcaInfo.samplingRate && hcaInfo.samplingRate <= 0x7fffff)) {
                throw CFormatException("Sampling rate is out of range.");
            }
        }

        // COMP or DEC
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            if (areMagicMatch(magic, Magic::COMPRESS)) {
                HCA_COMPRESS_HEADER hcaCompressHeader;
                ENSURE_READ_ALL(hcaCompressHeader);
                hcaInfo.blockSize = bswap(hcaCompressHeader.blockSize);
                hcaInfo.compR01 = hcaCompressHeader.r01;
                hcaInfo.compR02 = hcaCompressHeader.r02;
                hcaInfo.compR03 = hcaCompressHeader.r03;
                hcaInfo.compR04 = hcaCompressHeader.r04;
                hcaInfo.compR05 = hcaCompressHeader.r05;
                hcaInfo.compR06 = hcaCompressHeader.r06;
                hcaInfo.compR07 = hcaCompressHeader.r07;
                hcaInfo.compR08 = hcaCompressHeader.r08;
                if (!((hcaInfo.blockSize >= 8 && hcaInfo.blockSize <= 0xFFFF) || (hcaInfo.blockSize == 0))) {
                    throw CFormatException("Block size is out of range.");
                }
                if (!(hcaInfo.compR01 >= 0 && hcaInfo.compR01 <= hcaInfo.compR02 && hcaInfo.compR02 <= 0x1f)) {
                    throw CFormatException("Compression: r-fields are out of range.");
                }
            } else if (areMagicMatch(magic, Magic::DECODE)) {
                HCA_DECODE_HEADER hcaDecodeHeader;
                ENSURE_READ_ALL(hcaDecodeHeader);
                hcaInfo.blockSize = bswap(hcaDecodeHeader.blockSize);
                hcaInfo.compR01 = hcaDecodeHeader.r01;
                hcaInfo.compR02 = hcaDecodeHeader.r02;
                hcaInfo.compR03 = hcaDecodeHeader.r04;
                hcaInfo.compR04 = hcaDecodeHeader.r03;
                hcaInfo.compR05 = static_cast<uint16_t>(hcaDecodeHeader.count1 + 1);
                hcaInfo.compR06 = static_cast<uint16_t>((hcaDecodeHeader.enableCount2 ? hcaDecodeHeader.count2 : hcaDecodeHeader.count1) + 1);
                hcaInfo.compR07 = hcaInfo.compR05 - hcaInfo.compR06;
                hcaInfo.compR08 = 0;
            } else {
                throw CFormatException("Compression or Decode header is required.");
            }
        }

        // VBR
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            if (areMagicMatch(magic, Magic::VBR)) {
                HCA_VBR_HEADER hcaVbrHeader;
                ENSURE_READ_ALL(hcaVbrHeader);
                hcaInfo.vbrR01 = bswap(hcaVbrHeader.r01);
                hcaInfo.vbrR02 = bswap(hcaVbrHeader.r02);
            } else {
                hcaInfo.vbrR01 = hcaInfo.vbrR02 = 0;
            }
        }

        // ATH
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            if (areMagicMatch(magic, Magic::ATH)) {
                HCA_ATH_HEADER hcaAthHeader;
                ENSURE_READ_ALL(hcaAthHeader);
                hcaInfo.athType = hcaAthHeader.type;
            } else {
                hcaInfo.athType = static_cast<uint16_t>(hcaInfo.versionMajor < 2 ? 1 : 0);
            }
        }

        // LOOP
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            if (areMagicMatch(magic, Magic::LOOP)) {
                HCA_LOOP_HEADER hcaLoopHeader;
                ENSURE_READ_ALL(hcaLoopHeader);
                hcaInfo.loopExists = TRUE;
                hcaInfo.loopStart = bswap(hcaLoopHeader.loopStart);
                hcaInfo.loopEnd = bswap(hcaLoopHeader.loopEnd);
                hcaInfo.loopR01 = bswap(hcaLoopHeader.r01);
                hcaInfo.loopR02 = bswap(hcaLoopHeader.r02);
                if (!(0 <= hcaInfo.loopStart && hcaInfo.loopStart <= hcaInfo.loopEnd && hcaInfo.loopEnd < hcaInfo.blockCount)) {
                    throw CFormatException("Loop information is invalid.");
                }
            } else {
                hcaInfo.loopStart = hcaInfo.loopEnd = 0;
                hcaInfo.loopR01 = 0;
                hcaInfo.loopR02 = 0x400;
                hcaInfo.loopExists = FALSE;
            }
        }

        // CIPH
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            if (areMagicMatch(magic, Magic::CIPHER)) {
                HCA_CIPHER_HEADER hcaCipherHeader;
                ENSURE_READ_ALL(hcaCipherHeader);
                const auto cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(bswap(hcaCipherHeader.type));
                hcaInfo.cipherType = cipherType;
                if (!(cipherType == CGSS_HCA_CIPH_NO_CIPHER || cipherType == CGSS_HCA_CIPH_STATIC || cipherType == CGSS_HCA_CIPH_WITH_KEY)) {
                    throw CFormatException("Cipher type is invalid.");
                }
            } else {
                hcaInfo.cipherType = CGSS_HCA_CIPH_NO_CIPHER;
            }
        }

        // RVA (relative volume adjustment)
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            if (areMagicMatch(magic, Magic::RVA)) {
                HCA_RVA_HEADER hcaRvaHeader;
                ENSURE_READ_ALL(hcaRvaHeader);
                hcaInfo.rvaVolume = bswap(hcaRvaHeader.volume);
            } else {
                hcaInfo.rvaVolume = 1.0f;
            }
        }

        // COMM
        {
            CBinaryReader binaryReader(stream);
            auto magic = binaryReader.PeekUInt32LE();
            memset(hcaInfo.comment, 0, 0x100);
            if (areMagicMatch(magic, Magic::COMMENT)) {
                HCA_COMMENT_HEADER hcaCommentHeader;
                ENSURE_READ_ALL(hcaCommentHeader);
                hcaInfo.commentLength = hcaCommentHeader.length;
                strncpy(hcaInfo.comment, hcaCommentHeader.comment, hcaInfo.commentLength);
            } else {
                hcaInfo.commentLength = 0;
            }
        }

        if (!hcaInfo.compR03) {
            hcaInfo.compR03 = 1;
        }
        if (!(hcaInfo.compR01 == 1 && hcaInfo.compR02 == 0xf)) {
            throw CFormatException("Compression/Decode: r-fields are out of range.");
        }
        hcaInfo.compR09 = ceil2(hcaInfo.compR05 - (hcaInfo.compR06 + hcaInfo.compR07), hcaInfo.compR08);

#undef ENSURE_READ_ALL
#undef ENSURE_READ_ALL_BUFFER
        stream->Seek(hcaInfo.dataOffset, StreamSeekOrigin::Begin);
    }

    bool_t CHcaFormatReader::IsReadable() const {
        return TRUE;
    }

    bool_t CHcaFormatReader::IsWritable() const {
        return FALSE;
    }

    bool_t CHcaFormatReader::IsSeekable() const {
        return TRUE;
    }

    void CHcaFormatReader::Flush() {
        throw CInvalidOperationException();
    }

    void CHcaFormatReader::SetLength(uint64_t value) {
        throw CInvalidOperationException();
    }

    uint32_t CHcaFormatReader::Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) {
        throw CInvalidOperationException();
    }

    void CHcaFormatReader::PrintHcaInfo() {
        const auto &hcaInfo = _hcaInfo;
        using namespace std;

        cout << "General:" << endl;
        cout << "  Version: " << hcaInfo.versionMajor << "." << hcaInfo.versionMinor << endl;
        cout << "  Channels: " << hcaInfo.channelCount << endl;
        cout << "  Sampling rate: " << hcaInfo.samplingRate << endl;
        cout << "  Data offset: " << hcaInfo.dataOffset << endl;
        cout << "  Has loop: " << (hcaInfo.loopExists ? "Yes" : "No") << endl;
        cout << "Decoding:" << endl;
        cout << "  Number of blocks: " << hcaInfo.blockCount << endl;
        cout << "  Size of each block: " << hcaInfo.blockSize << endl;
        cout << "  Cipher type: " << hcaInfo.cipherType << endl;
        if (hcaInfo.loopExists) {
            cout << "Loop:" << endl;
            cout << "  Start index: " << hcaInfo.loopStart << endl;
            cout << "  End index: " << hcaInfo.loopEnd << endl;
        }
        cout << "Other:" << endl;
        cout << "  ATH: " << hcaInfo.athType << endl;
        cout << "  Volume adjustment: " << hcaInfo.rvaVolume << endl;
    }

    bool_t CHcaFormatReader::IsPossibleHcaStream(IStream *stream) {
        if (!stream) {
            return FALSE;
        }

        const auto pos = stream->GetPosition();

        try {
            NullHcaReader reader(stream);
        } catch (CException &ex) {
            return FALSE;
        } catch (std::runtime_error &err) {
            return FALSE;
        }

        stream->SetPosition(pos);

        return TRUE;
    }

CGSS_NS_END
