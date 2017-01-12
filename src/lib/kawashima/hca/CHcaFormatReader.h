#pragma once

#include "../../cgss_env.h"
#include "../../cgss_cdata.h"
#include "../../takamori/streams/CStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CHcaFormatReader : public CStream {

    __extends(CStream);

    public:

        CHcaFormatReader(IStream *baseStream);

        virtual ~CHcaFormatReader();

        /**
         * Retrieves the HCA meta information.
         * @param info Retrieved HCA information.
         * @return
         */
        void GetHcaInfo(HCA_INFO &info);

        virtual uint32_t Write(const void *buffer, uint32_t bufferSize, size_t offset, uint32_t count) override;

        virtual bool_t IsWritable() const override;

        virtual bool_t IsReadable() const override;

        virtual bool_t IsSeekable() const override;

        virtual void SetLength(uint64_t value) override;

        virtual void Flush() override;

    protected:

        static uint16_t ComputeChecksum(void *pData, uint32_t dwDataSize, uint16_t wInitSum);

        HCA_INFO _hcaInfo;

        IStream *_baseStream;

    private:

        CHcaFormatReader(const CHcaFormatReader &) = delete;

        void Initialize();

        void PrintHcaInfo();

        static const uint16_t *ChecksumTable;

    };

CGSS_NS_END
