#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CNotImplementedException : public CException {

    __extends(CException, CNotImplementedException);

    public:

        CNotImplementedException();

        CNotImplementedException(const CNotImplementedException &) = default;

        explicit CNotImplementedException(const char *message);

        explicit CNotImplementedException(const std::string &message);

    };

CGSS_NS_END
