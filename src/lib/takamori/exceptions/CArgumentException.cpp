#include "CArgumentException.h"

CGSS_NS_BEGIN

    CArgumentException::CArgumentException()
        : MyClass("") {
    }

    CArgumentException::CArgumentException(const char *message)
        : MyBase(CGSS_OP_INVALID_ARGUMENT, message) {
    }

    CArgumentException::CArgumentException(const std::string &message)
        : MyBase(CGSS_OP_INVALID_ARGUMENT, message) {
    }

CGSS_NS_END
