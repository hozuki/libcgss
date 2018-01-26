#pragma once

#include "../cgss_env.h"
#include "UTF_HEADER.h"
#include "UTF_ROW.h"

#define UTF_TABLE_MAX_NAME_LEN (1024)

// A C structure used to present CUtfTable.
typedef struct _UTF_TABLE {

    UTF_HEADER header;

    UTF_ROW *rows;

    bool_t isEncrypted;

    char tableName[UTF_TABLE_MAX_NAME_LEN];

} UTF_TABLE;
