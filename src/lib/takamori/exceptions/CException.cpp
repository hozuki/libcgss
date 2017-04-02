#include "CException.h"

CGSS_NS_BEGIN

    CException::CException()
        : MyClass("") {
    }

    CException::CException(CGSS_OP_RESULT result)
        : MyClass(result, "") {
    }

    CException::CException(const char *message)
        : MyClass(std::string(message)) {
    }

    CException::CException(const std::string &message)
        : MyClass(CGSS_OP_GENERIC_FAULT, message) {
    }

    CException::CException(CGSS_OP_RESULT result, const std::string &message)
        : _result(result), _message(message) {
    }

    const std::string CException::GetExceptionMessage() const {
        return _message;
    }

    CGSS_OP_RESULT CException::GetOpResult() const {
        return _result;
    }

CGSS_NS_END
