#include "CException.h"

CGSS_NS_BEGIN

    CException::CException()
        : CException("") {
    }

    CException::CException(CGSS_OP_RESULT result)
        : CException(result, "") {
    }

    CException::CException(const char *message)
        : CException(std::string(message)) {
    }

    CException::CException(const std::string &message)
        : CException(CGSS_OP_GENERIC_FAULT, message) {
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
