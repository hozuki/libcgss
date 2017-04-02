#include "CNotImplementedException.h"

CGSS_NS_BEGIN

    CNotImplementedException::CNotImplementedException()
        : MyClass("") {
    }

    CNotImplementedException::CNotImplementedException(const char *message)
        : MyBase(CGSS_OP_NOT_IMPLEMENTED, message) {
    }

    CNotImplementedException::CNotImplementedException(const std::string &message)
        : MyBase(CGSS_OP_NOT_IMPLEMENTED, message) {
    }

CGSS_NS_END
