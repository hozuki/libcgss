#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CNotImplementedException : public CException {

    __extends(CException);

    public:

        CNotImplementedException();

        CNotImplementedException(const CNotImplementedException &) = default;

        CNotImplementedException(const char *message);

        CNotImplementedException(const std::string &message);

    };

CGSS_NS_END
