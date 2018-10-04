#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    struct IStream;

    class CMemoryStream;

    class CGSS_EXPORT CAcbHelper final {

    PURE_STATIC(CAcbHelper);

    __root_class(CAcbHelper);

    public:

        static CMemoryStream *ExtractToNewStream(IStream *stream, uint64_t offset, uint32_t size);

        static uint64_t RoundUpToAlignment(uint64_t value, uint64_t alignment);

        static uint32_t RoundUpToAlignment(uint32_t value, uint32_t alignment);

        static int64_t RoundUpToAlignment(int64_t value, int64_t alignment);

        static int32_t RoundUpToAlignment(int32_t value, int32_t alignment);

    };

CGSS_NS_END
