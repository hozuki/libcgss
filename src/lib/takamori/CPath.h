#pragma once

#include <string>

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CPath final {

    PURE_STATIC(CPath);

    public:

        static std::string Combine(const std::string &basePath, const std::string &path1);

        static std::string GetExtension(const std::string &path);

        static std::string GetFileName(const std::string &path);

        static std::string GetDirectoryName(const std::string &path);

        static std::string GetFileNameWithoutExtension(const std::string &path);

    private:

        static std::string::size_type FindLastSlash(const std::string &path);

    };

CGSS_NS_END
