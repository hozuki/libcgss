#include "CArgumentException.h"

CGSS_NS_BEGIN

    CArgumentException::CArgumentException() noexcept
        : MyClass("") {
    }

    CArgumentException::CArgumentException(const char *message) noexcept
        : MyBase(CGSS_OP_INVALID_ARGUMENT, message) {
    }

    CArgumentException::CArgumentException(const std::string &message) noexcept
        : MyBase(CGSS_OP_INVALID_ARGUMENT, message) {
    }

    CArgumentException::CArgumentException(const CArgumentException &exception) noexcept
        : MyBase(exception) {
    }

CGSS_NS_END
