#pragma once

#include "../cgss_env.h"

CGSS_NS_BEGIN

    struct IStream;

    class CMemoryStream;

    class CGSS_EXPORT CAcbHelper final {

    PURE_STATIC(CAcbHelper);

    public:

        static CMemoryStream *ExtractToNewStream(IStream *stream, uint64_t offset, uint32_t size);

    };

CGSS_NS_END
