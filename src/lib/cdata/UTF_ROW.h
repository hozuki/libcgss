#pragma once

#include "../cgss_env.h"
#include "UTF_FIELD.h"

// Used for viewing only. Alternative is CUtfTable::UtfRow.
typedef struct _UTF_ROW {

    uint32_t baseOffset;

    UTF_FIELD *fields;

} UTF_ROW;
