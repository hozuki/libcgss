#include "CNotImplementedException.h"

CGSS_NS_BEGIN

    CNotImplementedException::CNotImplementedException() noexcept
        : MyClass("") {
    }

    CNotImplementedException::CNotImplementedException(const char *message) noexcept
        : MyBase(CGSS_OP_NOT_IMPLEMENTED, message) {
    }

    CNotImplementedException::CNotImplementedException(const std::string &message) noexcept
        : MyBase(CGSS_OP_NOT_IMPLEMENTED, message) {
    }

    CNotImplementedException::CNotImplementedException(const cgss::CNotImplementedException &exception) noexcept
        : MyBase(exception) {
    }

CGSS_NS_END
