#include "CException.h"

CGSS_NS_BEGIN

    CException::CException() noexcept
        : MyClass("") {
    }

    // Intended
    CException::CException(const CException &) noexcept
        : MyBase(), _result(CGSS_OP_GENERIC_FAULT) {
    }

    CException::CException(CGSS_OP_RESULT result) noexcept
        : MyClass(result, "") {
    }

    CException::CException(const char *message) noexcept
        : MyClass(std::string(message)) {
    }

    CException::CException(const std::string &message) noexcept
        : MyClass(CGSS_OP_GENERIC_FAULT, message) {
    }

    CException::CException(CGSS_OP_RESULT result, std::string message) noexcept
        : MyBase(), _result(result), _message(std::move(message)) {
    }

    CException &CException::operator=(const CException &exception) noexcept {
        _result = exception._result;
        _message = exception._message;

        return *this;
    }

    const std::string &CException::GetExceptionMessage() const noexcept {
        return _message;
    }

    CGSS_OP_RESULT CException::GetOpResult() const noexcept {
        return _result;
    }

    const char *CException::what() const noexcept {
        return _message.c_str();
    }

CGSS_NS_END
