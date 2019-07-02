#include "CFormatException.h"

CGSS_NS_BEGIN

    CFormatException::CFormatException() noexcept
        : MyClass("") {
    }

    CFormatException::CFormatException(const char *message) noexcept
        : MyBase(CGSS_OP_FORMAT_ERROR, message) {
    }

    CFormatException::CFormatException(const std::string &message) noexcept
        : MyBase(CGSS_OP_FORMAT_ERROR, message) {
    }

    CFormatException::CFormatException(const CFormatException &exception) noexcept
        : MyBase(exception) {
    }

CGSS_NS_END
