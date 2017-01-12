#pragma once

#include <string>
#include "../../cgss_env.h"
#include "../../cgss_cenum.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CException {

    public:

        CException();

        CException(const CException &) = default;

        CException(CGSS_OP_RESULT result);

        CException(const char *message);

        CException(const std::string &message);

        CException(CGSS_OP_RESULT result, const std::string &message);

        virtual ~CException() = default;

        virtual const std::string GetExceptionMessage() const;

        virtual CGSS_OP_RESULT GetOpResult() const;

    protected:

        std::string _message;
        const CGSS_OP_RESULT _result;

    };

CGSS_NS_END
