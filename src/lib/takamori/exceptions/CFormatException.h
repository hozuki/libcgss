#pragma once

#include "../../cgss_env.h"
#include "CException.h"

CGSS_NS_BEGIN ;

    class CGSS_EXPORT CFormatException : public CException {

    __extends(CException, CFormatException);

    public:

        CFormatException() noexcept;

        CFormatException(const CFormatException &) noexcept;

        explicit CFormatException(const char *message) noexcept;

        explicit CFormatException(const std::string &message) noexcept;

    };

CGSS_NS_END;
