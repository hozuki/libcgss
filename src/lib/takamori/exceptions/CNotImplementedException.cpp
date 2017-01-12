#include "CNotImplementedException.h"

CGSS_NS_BEGIN

    CNotImplementedException::CNotImplementedException()
        : CNotImplementedException("") {
    }

    CNotImplementedException::CNotImplementedException(const char *message)
        : super(CGSS_OP_NOT_IMPLEMENTED, message) {
    }

    CNotImplementedException::CNotImplementedException(const std::string &message)
        : CException(CGSS_OP_NOT_IMPLEMENTED, message) {
    }

CGSS_NS_END
