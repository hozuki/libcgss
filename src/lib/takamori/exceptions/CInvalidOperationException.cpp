#include "CInvalidOperationException.h"

CGSS_NS_BEGIN

    CInvalidOperationException::CInvalidOperationException()
        : MyClass("") {
    }

    CInvalidOperationException::CInvalidOperationException(const char *message)
        : MyBase(CGSS_OP_INVALID_OPERATION, message) {
    }

    CInvalidOperationException::CInvalidOperationException(const std::string &message)
        : MyBase(CGSS_OP_INVALID_OPERATION, message) {
    }

CGSS_NS_END
