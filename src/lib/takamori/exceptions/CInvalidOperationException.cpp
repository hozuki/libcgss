#include "CInvalidOperationException.h"

CGSS_NS_BEGIN

    CInvalidOperationException::CInvalidOperationException()
        : CInvalidOperationException("") {
    }

    CInvalidOperationException::CInvalidOperationException(const char *message)
        : super(CGSS_OP_INVALID_OPERATION, message) {
    }

    CInvalidOperationException::CInvalidOperationException(const std::string &message)
        : CException(CGSS_OP_INVALID_OPERATION, message) {
    }

CGSS_NS_END
