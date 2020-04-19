#pragma once

#include <stdint.h>

struct _UTF_FIELD;

#pragma pack(push)
#pragma pack(1)

// Used for viewing only. Alternative is CUtfTable::UtfRow.
typedef struct _UTF_ROW {

    uint32_t baseOffset;

    struct _UTF_FIELD *fields;

} UTF_ROW;

#pragma pack(pop)
