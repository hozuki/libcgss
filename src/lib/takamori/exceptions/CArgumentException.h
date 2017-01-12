#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CArgumentException : public CException {

    __extends(CException);

    public:

        CArgumentException();

        CArgumentException(const CArgumentException &) = default;

        CArgumentException(const char *message);

        CArgumentException(const std::string &message);

    };

CGSS_NS_END
