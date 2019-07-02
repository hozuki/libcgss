#pragma once

#include "CException.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CInvalidOperationException : public CException {

    __extends(CException, CInvalidOperationException);

    public:

        CInvalidOperationException() noexcept;

        CInvalidOperationException(const CInvalidOperationException &) noexcept;

        explicit CInvalidOperationException(const char *message) noexcept;

        explicit CInvalidOperationException(const std::string &message) noexcept;

    };

CGSS_NS_END
