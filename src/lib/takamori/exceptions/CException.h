#pragma once

#include <string>
#include "../../cgss_env.h"
#include "../../cgss_cenum.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CException {

    __root_class(CException);

    public:

        CException();

        CException(const CException &) = default;

        explicit CException(CGSS_OP_RESULT result);

        explicit CException(const char *message);

        explicit CException(const std::string &message);

        CException(CGSS_OP_RESULT result, const std::string &message);

        virtual ~CException() = default;

        virtual const std::string GetExceptionMessage() const;

        virtual CGSS_OP_RESULT GetOpResult() const;

    protected:

        std::string _message;
        const CGSS_OP_RESULT _result;

    };

CGSS_NS_END
