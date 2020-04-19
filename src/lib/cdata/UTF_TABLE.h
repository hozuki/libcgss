#pragma once

#include <stdint.h>

#include "bool_t.h"

struct _UTF_HEADER;
struct _UTF_ROW;

#define UTF_TABLE_MAX_NAME_LEN (1024)

#pragma pack(push)
#pragma pack(1)

// A C structure used to present CUtfTable.
typedef struct _UTF_TABLE {

    struct _UTF_HEADER header;

    struct _UTF_ROW *rows;

    bool_t isEncrypted;

    char tableName[UTF_TABLE_MAX_NAME_LEN];

} UTF_TABLE;

#pragma pack(pop)
