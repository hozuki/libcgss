#pragma once

#include <string>

#include "../cgss_env.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CFileSystem final {

    PURE_STATIC(CFileSystem);

    public:

        static bool_t FileExists(const std::string &path);

        static bool_t DirectoryExists(const std::string &path);

        static bool_t FileExists(const char *path);

        static bool_t DirectoryExists(const char *path);

        static bool_t MkDir(const std::string &path);

        static bool_t MkDir(const char *path);

        static bool_t RmFile(const std::string &path);

        static bool_t RmFile(const char *path);

    };

CGSS_NS_END
