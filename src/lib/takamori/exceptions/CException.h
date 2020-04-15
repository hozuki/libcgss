#pragma once

#include <string>
#include "../../cgss_env.h"
#include "../../cgss_cenum.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CException : public std::exception {

    __extends(std::exception, CException);

    public:

        CException() noexcept;

        CException(const CException &) noexcept;

        CException& operator=(const CException &) noexcept;

        explicit CException(CGSS_OP_RESULT result) noexcept;

        explicit CException(const char *message) noexcept;

        explicit CException(const std::string &message) noexcept;

        CException(CGSS_OP_RESULT result, std::string message) noexcept;

        virtual ~CException() noexcept = default;

        virtual const std::string &GetExceptionMessage() const noexcept;

        virtual CGSS_OP_RESULT GetOpResult() const noexcept;

        const char *what() const noexcept override;

    protected:

        std::string _message;
        CGSS_OP_RESULT _result;

    };

CGSS_NS_END
