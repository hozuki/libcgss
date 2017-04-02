#include "CFormatException.h"

CGSS_NS_BEGIN

    CFormatException::CFormatException()
            : MyClass("") {
    }

    CFormatException::CFormatException(const char *message)
            : MyBase(CGSS_OP_FORMAT_ERROR, message) {
    }

    CFormatException::CFormatException(const std::string &message)
            : MyBase(CGSS_OP_FORMAT_ERROR, message) {
    }

CGSS_NS_END
