#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CArgumentException : public CException {

    __extends(CException, CArgumentException);

    public:

        CArgumentException() noexcept;

        CArgumentException(const CArgumentException &) noexcept;

        explicit CArgumentException(const char *message) noexcept;

        explicit CArgumentException(const std::string &message) noexcept;

    };

CGSS_NS_END
