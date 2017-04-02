#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CInvalidOperationException : public CException {

    __extends(CException, CInvalidOperationException);

    public:

        CInvalidOperationException();

        CInvalidOperationException(const CInvalidOperationException &) = default;

        CInvalidOperationException(const char *message);

        CInvalidOperationException(const std::string &message);

    };

CGSS_NS_END
