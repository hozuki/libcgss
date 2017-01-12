#pragma once

#include "../../cgss_data.h"
#include "CHcaFormatReader.h"

CGSS_NS_BEGIN

    class CHcaCipher;

    class CGSS_EXPORT CHcaCipherConverter : public CHcaFormatReader {

    __extends(CHcaFormatReader);

    public:

        /**
          * Creates a new HCA cipher converter.
          * @param stream Source stream.
          * @param cryptFrom Source cipher type.
          * @param cryptTo Wanted cipher type.
          * @return
          */
        CHcaCipherConverter(CStream *stream, const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo);

        virtual ~CHcaCipherConverter();

    private:

        void ConvertBlock(uint8_t *pData, uint32_t dwBlockSize);

        void FixBlock(uint8_t *pData, uint32_t dwBlockSize);

        void FixHeader(uint8_t *pData, uint32_t dwFileSize, uint32_t dwDataOffset);

        CHcaCipher *_cipherFrom, *_cipherTo;

        HCA_CIPHER_CONFIG _ccFrom, _ccTo;

    private:

        CHcaCipherConverter(const CHcaCipherConverter &) = delete;

    };

CGSS_NS_END
