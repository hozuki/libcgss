#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CNotImplementedException : public CException {

    __extends(CException, CNotImplementedException);

    public:

        CNotImplementedException() noexcept;

        CNotImplementedException(const CNotImplementedException &) noexcept;

        explicit CNotImplementedException(const char *message) noexcept;

        explicit CNotImplementedException(const std::string &message) noexcept;

    };

CGSS_NS_END
