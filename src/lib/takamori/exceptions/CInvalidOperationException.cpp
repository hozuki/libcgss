#include "CInvalidOperationException.h"

CGSS_NS_BEGIN

    CInvalidOperationException::CInvalidOperationException() noexcept
        : MyClass("") {
    }

    CInvalidOperationException::CInvalidOperationException(const char *message) noexcept
        : MyBase(CGSS_OP_INVALID_OPERATION, message) {
    }

    CInvalidOperationException::CInvalidOperationException(const std::string &message) noexcept
        : MyBase(CGSS_OP_INVALID_OPERATION, message) {
    }

    CInvalidOperationException::CInvalidOperationException(const CInvalidOperationException &exception) noexcept
        : MyBase(exception) {
    }

CGSS_NS_END
