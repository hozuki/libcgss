#include "CArgumentException.h"

CGSS_NS_BEGIN

    CArgumentException::CArgumentException()
        : CArgumentException("") {
    }

    CArgumentException::CArgumentException(const char *message)
        : super(CGSS_OP_INVALID_ARGUMENT, message) {
    }

    CArgumentException::CArgumentException(const std::string &message)
        : super(CGSS_OP_INVALID_ARGUMENT, message) {
    }

CGSS_NS_END
