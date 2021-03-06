#include <stdio.h>
#include <inttypes.h>

#include "../../lib/cgss_api.h"

#define INDENT_VALUE (2u)

static void print_table(const char *file_name, UTF_TABLE *table);

static void print_table_recursive(UTF_TABLE *table, uint32_t indent);

static void printf_indent(uint32_t indent);

static void fprintf_indent(FILE *out, uint32_t indent);

static void print_app_title(FILE *out);

static void print_help();

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    const char *file_name = argv[1];

    if (!cgssHelperFileExists(file_name)) {
        fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", file_name);
        return -1;
    }

#define ERROR_BUFFER_SIZE (100)
    char error_buffer[ERROR_BUFFER_SIZE] = {'\0'};

    CGSS_HANDLE stream;

    CGSS_OP_RESULT r = cgssCreateFileStream3(file_name, CGSS_FILE_MODE_OPEN_EXISTING, CGSS_FILE_ACCESS_READ, &stream);

    if (!CGSS_OP_SUCCEEDED(r)) {
        cgssGetOpResultString(r, error_buffer, ERROR_BUFFER_SIZE);
        fprintf(stderr, "Error when creating stream, code = %d (%s)\nDetail: %s\n", r, error_buffer, cgssGetLastErrorMessage());
        return r;
    }

    UTF_TABLE *table;

    r = cgssUtfReadTable(stream, 0, &table);

    if (CGSS_OP_SUCCEEDED(r)) {
        print_table(file_name, table);

        cgssUtfFreeTable(table);
        table = NULL;
    } else {
        cgssGetOpResultString(r, error_buffer, ERROR_BUFFER_SIZE);
        fprintf(stderr, "Error when reading UTF table, code = %d (%s)\nDetail: %s\n", r, error_buffer, cgssGetLastErrorMessage());
    }

    cgssCloseHandle(stream);

    return 0;
}

#define BOOL_STR(v) ((v) ? "yes" : "no")
#define PRINT_INDENT() printf_indent(indent)

static void print_table(const char *file_name, UTF_TABLE *table) {
    print_app_title(stdout);

    fprintf(stdout, "# File: %s\n\n", file_name);

    print_table_recursive(table, 0);
}

static void print_table_recursive(UTF_TABLE *table, uint32_t indent) {
    fprintf(stdout, "{\n");
    indent += INDENT_VALUE;

    PRINT_INDENT();
    fprintf(stdout, "@%s (encrypted: %s, field count: %" PRIu32 ", row count: %" PRIu32 ")\n", table->tableName,
            BOOL_STR(table->isEncrypted), table->header.fieldCount, table->header.rowCount);

    for (uint32_t i = 0; i < table->header.rowCount; ++i) {
        UTF_ROW *currentRow = table->rows + i;

        PRINT_INDENT();
        fprintf(stdout, "@%s [%" PRIu32 "] = {\n", table->tableName, i);
        indent += INDENT_VALUE;

        if (table->header.fieldCount == 0) {
            continue;
        }

        for (int32_t j = 0; j < table->header.fieldCount; ++j) {
            UTF_FIELD *currentField = currentRow->fields + j;

            const char *columnTypeName;

            switch (currentField->type) {
                case CGSS_UTF_COLUMN_TYPE_U8:
                    columnTypeName = "U8";
                    break;
                case CGSS_UTF_COLUMN_TYPE_S8:
                    columnTypeName = "S8";
                    break;
                case CGSS_UTF_COLUMN_TYPE_U16:
                    columnTypeName = "U16";
                    break;
                case CGSS_UTF_COLUMN_TYPE_S16:
                    columnTypeName = "S16";
                    break;
                case CGSS_UTF_COLUMN_TYPE_U32:
                    columnTypeName = "U32";
                    break;
                case CGSS_UTF_COLUMN_TYPE_S32:
                    columnTypeName = "S32";
                    break;
                case CGSS_UTF_COLUMN_TYPE_U64:
                    columnTypeName = "U64";
                    break;
                case CGSS_UTF_COLUMN_TYPE_S64:
                    columnTypeName = "S64";
                    break;
                case CGSS_UTF_COLUMN_TYPE_R32:
                    columnTypeName = "R32";
                    break;
                case CGSS_UTF_COLUMN_TYPE_R64:
                    columnTypeName = "R64";
                    break;
                case CGSS_UTF_COLUMN_TYPE_STRING:
                    columnTypeName = "STR";
                    break;
                case CGSS_UTF_COLUMN_TYPE_DATA:
                    columnTypeName = "DAT";
                    break;
                default:
                    columnTypeName = "UNKNOWN";
                    break;
            }

            PRINT_INDENT();
            fprintf(stdout, "%08" PRIx32 " (row+%08" PRIx32 ") %2" PRIx32 " %s [%s] =", currentField->offset, currentField->offsetInRow,
                    ((uint32_t)currentField->storage | (uint32_t)currentField->type), currentField->name, columnTypeName);

            const char *constantTypeStr;

            if (currentField->storage == CGSS_UTF_COLUMN_STORAGE_CONST) {
                constantTypeStr = " constant";
            } else if (currentField->storage == CGSS_UTF_COLUMN_STORAGE_CONST2) {
                constantTypeStr = " constant2";
            } else if (currentField->storage == CGSS_UTF_COLUMN_STORAGE_ZERO) {
                constantTypeStr = " zero";
            } else {
                constantTypeStr = "";
            }

            UTF_TABLE *tbl;

            switch (currentField->type) {
                case CGSS_UTF_COLUMN_TYPE_U8:
                    printf("%s unsigned %" PRIu8, constantTypeStr, currentField->value.u8);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S8:
                    printf("%s signed %" PRId8, constantTypeStr, currentField->value.s8);
                    break;
                case CGSS_UTF_COLUMN_TYPE_U16:
                    printf("%s unsigned %" PRIu16, constantTypeStr, currentField->value.u16);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S16:
                    printf("%s signed %" PRId16, constantTypeStr, currentField->value.s16);
                    break;
                case CGSS_UTF_COLUMN_TYPE_U32:
                    printf("%s unsigned %" PRIu32, constantTypeStr, currentField->value.u32);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S32:
                    printf("%s signed %" PRId32, constantTypeStr, currentField->value.s32);
                    break;
                case CGSS_UTF_COLUMN_TYPE_U64:
                    printf("%s unsigned %" PRIu64, constantTypeStr, currentField->value.u64);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S64:
                    printf("%s signed %" PRId64, constantTypeStr, currentField->value.s64);
                    break;
                case CGSS_UTF_COLUMN_TYPE_R32:
                    printf("%s %f", constantTypeStr, currentField->value.r32);
                    break;
                case CGSS_UTF_COLUMN_TYPE_R64:
                    printf("%s %lf", constantTypeStr, currentField->value.r64);
                    break;
                case CGSS_UTF_COLUMN_TYPE_STRING:
                    if (currentField->value.str) {
                        printf("%s \"%s\"", constantTypeStr, currentField->value.str);
                    } else {
                        printf("%s null", constantTypeStr);
                    }
                    break;
                case CGSS_UTF_COLUMN_TYPE_DATA:
                    if (cgssUtfTryParseTable(currentField->value.data.ptr, currentField->value.data.size, &tbl)) {
                        printf(" ");
                        print_table_recursive(tbl, indent + INDENT_VALUE);

                        cgssUtfFreeTable(tbl);
                    } else {
                        printf("%s (size %" PRIu32 " = 0x%" PRIx32 ")", constantTypeStr, currentField->value.data.size, currentField->value.data.size);
                    }
                    break;
                default:
                    break;
            }

            fprintf(stdout, "\n");
        }

        indent -= INDENT_VALUE;
        PRINT_INDENT();
        fprintf(stdout, "}\n");
    }
    indent -= INDENT_VALUE;

    PRINT_INDENT();
    fprintf(stdout, "}");
}

static void print_app_title(FILE *out) {
    static const char *app_description = "utftable: UTF schema reader";
    static const char *version_string = "v0.1";

    fprintf(out, "%s %s\n\n", app_description, version_string);
}

static void print_help() {
    print_app_title(stderr);

    static const char *help_message =
        "Usage:\n"
        "\n"
        "utftable <file>\n";

    fprintf(stderr, "%s", help_message);
}

static void printf_indent(uint32_t indent) {
    fprintf_indent(stdout, indent);
}

static void fprintf_indent(FILE *out, uint32_t indent) {
    if (indent > 0) {
        fprintf(out, "%*s", indent, "");
    }
}
