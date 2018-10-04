#pragma once

#include "../../cgss_env.h"
#include "CException.h"

CGSS_NS_BEGIN ;

    class CGSS_EXPORT CFormatException : public CException {

    __extends(CException, CFormatException);

    public:

        CFormatException();

        CFormatException(const CFormatException &) = default;

        explicit CFormatException(const char *message);

        explicit CFormatException(const std::string &message);

    };

CGSS_NS_END;
