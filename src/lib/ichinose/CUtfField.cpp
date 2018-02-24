#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "CUtfField.h"

CGSS_NS_BEGIN

    CUtfField::CUtfField() {
        memset(this, 0, sizeof(UTF_FIELD));
    }

    CUtfField::CUtfField(const UTF_FIELD &tpl) {
        memcpy(this, &tpl, sizeof(UTF_FIELD));

        memset(name, 0, sizeof(name));
        strncpy(name, tpl.name, UTF_FIELD_MAX_NAME_LEN);

        switch (type) {
            case CGSS_UTF_COLUMN_TYPE_DATA:
                if (tpl.value.data.ptr && tpl.value.data.size > 0) {
                    value.data.ptr = malloc(tpl.value.data.size);
                    memcpy(value.data.ptr, tpl.value.data.ptr, tpl.value.data.size);
                }
                break;
            case CGSS_UTF_COLUMN_TYPE_STRING: {
                const auto strLength = tpl.value.str ? strlen(tpl.value.str) : 0;
                if (strLength > 0) {
                    value.str = (char *)malloc(strLength + 1);
                    strncpy(value.str, tpl.value.str, strLength);
                    value.str[strLength] = '\0';
                }
                break;
            }
            default:
                break;
        }
    }

    CUtfField::~CUtfField() {
        ReleaseData();
        memset(&value, 0, sizeof(value));
    }

    void CUtfField::SetName(const char *name) {
        strncpy(this->name, name, UTF_FIELD_MAX_NAME_LEN);
    }

    void CUtfField::ReleaseData() {
        switch (type) {
            case CGSS_UTF_COLUMN_TYPE_DATA:
                if (value.data.ptr) {
                    free(value.data.ptr);
                    value.data.ptr = nullptr;
                    value.data.size = 0;
                }
                break;
            case CGSS_UTF_COLUMN_TYPE_STRING:
                if (value.str) {
                    free(value.str);
                    value.str = nullptr;
                }
                break;
            default:
                break;
        }
    }

    void CUtfField::SetValue(const void *data, const uint32_t size, const uint32_t fieldOffset) {
        ReleaseData();
        if (data && size > 0) {
            value.data.ptr = malloc(size);
            memcpy(value.data.ptr, data, size);
        }
        value.data.size = size;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_DATA;
    }

    void CUtfField::SetValue(const char *str, uint32_t fieldOffset) {
        ReleaseData();
        const auto strLength = str ? strlen(str) : 0;

        value.str = (char *)malloc(strLength + 1);
        strncpy(value.str, str, strLength);
        value.str[strLength] = '\0';

        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_STRING;
    }

    void CUtfField::SetValue(const uint8_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.u8 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_U8;
    }

    void CUtfField::SetValue(const int8_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.s8 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_S8;
    }

    void CUtfField::SetValue(const uint16_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.u16 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_U16;
    }

    void CUtfField::SetValue(const int16_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.s16 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_S16;
    }

    void CUtfField::SetValue(const uint32_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.u32 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_U32;
    }

    void CUtfField::SetValue(const int32_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.s32 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_S32;
    }

    void CUtfField::SetValue(const uint64_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.u64 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_U64;
    }

    void CUtfField::SetValue(const int64_t v, uint32_t fieldOffset) {
        ReleaseData();
        value.s64 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_S64;
    }

    void CUtfField::SetValue(const float v, uint32_t fieldOffset) {
        ReleaseData();
        value.r32 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_R32;
    }

    void CUtfField::SetValue(const double v, uint32_t fieldOffset) {
        ReleaseData();
        value.r64 = v;
        offset = fieldOffset;
        type = CGSS_UTF_COLUMN_TYPE_R64;
    }

CGSS_NS_END
