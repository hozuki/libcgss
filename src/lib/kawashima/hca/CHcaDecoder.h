#pragma once

#include <map>
#include "../../cgss_data.h"
#include "CHcaFormatReader.h"

CGSS_NS_BEGIN

    class CHcaCipher;

    class CHcaAth;

    class CHcaChannel;

    class CGSS_EXPORT CHcaDecoder : public CHcaFormatReader {

    __extends(CHcaFormatReader, CHcaDecoder);

    public:

        explicit CHcaDecoder(IStream *stream);

        CHcaDecoder(IStream *stream, const HCA_DECODER_CONFIG &decoderConfig);

        CHcaDecoder(const CHcaDecoder &) = delete;

        virtual ~CHcaDecoder();

        uint32_t Read(void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        uint64_t GetPosition() override;

        void SetPosition(uint64_t value) override;

        uint64_t GetLength() override;

    private:

        void InitializeExtra();

        /**
         * Generate a wave header for decoded file.
         * @remarks You can use GetWaveHeaderSize() to determine the header size before trying to get wave header data.
         * @see ComputeWaveHeaderSize
         * @return
         */
        const uint8_t *GenerateWaveHeader();

        /**
         * Computes the minimum size required for generated wave header.
         * @return Computed size.
         */
        uint32_t GetWaveHeaderSize();

        /**
         * Continue to decode upcoming blocks, from HCA to wave audio, and write decoded data to data buffer.
         * @remarks You can use ComputeWaveBlockSize() to determine the minimum size for the data buffer before trying to decode.
         * Decoded data are in blocks. This function will try to decode the audio as much as possible when there is still data
         * and the buffer space left is still enough for a block.
         * @see ComputeWaveBlockSize
         * @return
         */
        const uint8_t *DecodeBlock(uint32_t blockIndex);

        /**
         * Computes the minimum size required for decoded wave data block.
         * @return Computed size.
         */
        uint32_t GetWaveBlockSize();

        /**
         * Map a linear position to a looped position, considering looping range.
         * @param linearPosition The wave stream position in linear order.
         * @return Mapped position. It falls in [0, waveHeaderSize + totalWaveBlocks * waveBlockSize].
         */
        uint64_t MapLoopedPosition(uint64_t linearPosition);

        std::map<uint32_t, const uint8_t *> _decodedBlocks;

        static const uint32_t ChannelCount = 0x10;

        CHcaAth *_ath;
        CHcaCipher *_cipher;
        HCA_DECODER_CONFIG _decoderConfig;
        CHcaChannel *_channels[ChannelCount];
        uint32_t _waveHeaderSize;
        uint8_t *_waveHeaderBuffer;
        uint32_t _waveBlockSize;
        uint8_t *_hcaBlockBuffer;
        // Position measured by wave output.
        uint64_t _position;

    };

CGSS_NS_END
