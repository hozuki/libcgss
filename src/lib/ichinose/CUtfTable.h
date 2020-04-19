#pragma once

#include <string>
#include <vector>
#include <map>
#include "../cgss_env.h"
#include "../takamori/streams/IStream.h"
#include "../cdata/UTF_HEADER.h"
#include "../cdata/UTF_FIELD.h"
#include "../cdata/UTF_TABLE.h"

CGSS_NS_BEGIN

    class CUtfReader;

    class CMemoryStream;

    class CGSS_EXPORT CUtfTable {

    __root_class(CUtfTable);

    public:

        CUtfTable(IStream *stream, uint64_t streamOffset);

        virtual ~CUtfTable();

        void GetHeader(UTF_HEADER &header) const;

        const UTF_HEADER GetHeader() const;

        IStream *GetStream() const;

        bool_t IsEncrypted() const;

        typedef struct {
            uint32_t baseOffset;
            std::vector<UTF_FIELD *> fields;
        } UtfRow;

        const std::vector<CUtfTable::UtfRow> &GetRows() const;

        const char *GetName() const;

        bool_t GetFieldOffset(uint32_t rowIndex, const char *fieldName, uint64_t *offset) const;

        bool_t GetFieldSize(uint32_t rowIndex, const char *fieldName, uint32_t *size) const;

    protected:

        CUtfReader *GetReader() const;

        std::vector<CUtfTable::UtfRow> _rows;

        virtual void Initialize();

    private:

        bool_t CheckEncryption(const uint8_t *magic);

        static bool_t GetKeysForEncryptedUtfTable(const uint8_t *magic, _OUT_ uint8_t *seed, _OUT_ uint8_t *incr);

        static void ReadUtfHeader(IStream *stream, UTF_HEADER &header, char *tableNameBuffer);

        static void ReadUtfHeader(IStream *stream, uint64_t streamOffset, UTF_HEADER &header, char *tableNameBuffer);

        void InitializeUtfSchema(IStream *sourceStream, CMemoryStream *tableDataStream, uint64_t schemaOffset);

        CMemoryStream *GetTableDataStream();

        UTF_HEADER _utfHeader;
        IStream *_stream;
        bool_t _isEncrypted;
        uint64_t _streamOffset;
        CUtfReader *_utfReader;
        char _tableName[UTF_TABLE_MAX_NAME_LEN];

    };

CGSS_NS_END
