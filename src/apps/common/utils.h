#pragma once

#include <string>

#include "../../lib/cgss_api.h"

#ifdef __cplusplus

namespace cgss {

    class common_utils final {

    public:
        common_utils() = delete;

        ~common_utils() = delete;

        common_utils(const common_utils &) = delete;

        common_utils(const common_utils &&) = delete;

        common_utils &operator=(const common_utils &) = delete;

        common_utils &operator=(const common_utils &&) = delete;

    public:

        static std::string ReplaceExtension(const std::string &s, const std::string &oldExt, const std::string &newExt);

        static std::string ReplaceAnyExtension(const std::string &s, const std::string &newExt);

        static void CopyStream(cgss::IStream *src, cgss::IStream *dst);

    };

}

#endif
