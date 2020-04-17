#pragma once

#include "../../cgss_env.h"
#include "IStream.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CStreamExtensions {

    public:

        CStreamExtensions() = delete;

        CStreamExtensions(const CStreamExtensions &) = delete;

        CStreamExtensions(CStreamExtensions &&) = delete;

        CStreamExtensions &operator=(const CStreamExtensions &) = delete;

        CStreamExtensions &operator=(CStreamExtensions &&) = delete;

        static void ReadNullEndedString(IStream *stream, char *buffer, size_t bufferSize);

    };

CGSS_NS_END
