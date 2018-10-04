#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CArgumentException : public CException {

    __extends(CException, CArgumentException);

    public:

        CArgumentException();

        CArgumentException(const CArgumentException &) = default;

        explicit CArgumentException(const char *message);

        explicit CArgumentException(const std::string &message);

    };

CGSS_NS_END
