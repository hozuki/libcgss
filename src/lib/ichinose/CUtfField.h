#pragma once

#include "../cgss_env.h"
#include "../cdata/UTF_FIELD.h"

CGSS_NS_BEGIN

    class CGSS_EXPORT CUtfField final : public UTF_FIELD {

    __extends(UTF_FIELD, CUtfField);

    public:

        CUtfField();

        explicit CUtfField(const UTF_FIELD &tpl);

        ~CUtfField();

        void SetName(const char *name);

        void SetValue(const void *data, uint32_t size, uint32_t fieldOffset);

        void SetValue(const char *str, uint32_t fieldOffset);

        void SetValue(uint8_t v, uint32_t fieldOffset);

        void SetValue(int8_t v, uint32_t fieldOffset);

        void SetValue(uint16_t v, uint32_t fieldOffset);

        void SetValue(int16_t v, uint32_t fieldOffset);

        void SetValue(uint32_t v, uint32_t fieldOffset);

        void SetValue(int32_t v, uint32_t fieldOffset);

        void SetValue(uint64_t v, uint32_t fieldOffset);

        void SetValue(int64_t v, uint32_t fieldOffset);

        void SetValue(float v, uint32_t fieldOffset);

        void SetValue(double v, uint32_t fieldOffset);

    private:

        void ReleaseData();

    };


CGSS_NS_END
