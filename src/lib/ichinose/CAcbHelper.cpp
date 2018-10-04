#include "CAcbHelper.h"
#include "../takamori/streams/CMemoryStream.h"

CGSS_NS_BEGIN

    CMemoryStream *CAcbHelper::ExtractToNewStream(IStream *stream, uint64_t offset, uint32_t size) {
        const auto originalPosition = stream->GetPosition();

        stream->Seek(offset, StreamSeekOrigin::Begin);

        auto *memory = new CMemoryStream(size, FALSE);
        auto bytesLeft = size;
        const auto bufferSize = 1024;
        auto *buffer = static_cast<uint8_t *>(malloc(bufferSize));

        do {
            const auto read = stream->Read(buffer, bufferSize, 0, bytesLeft);

            memory->Write(buffer, bufferSize, 0, read);
            bytesLeft -= read;
        } while (bytesLeft > 0);

        free(buffer);
        stream->Seek(originalPosition, StreamSeekOrigin::Begin);
        memory->Seek(0, StreamSeekOrigin::Begin);

        return memory;
    }

    uint64_t CAcbHelper::RoundUpToAlignment(uint64_t value, uint64_t alignment) {
        return (value + alignment - 1) / alignment * alignment;
    }

    uint32_t CAcbHelper::RoundUpToAlignment(uint32_t value, uint32_t alignment) {
        return (value + alignment - 1) / alignment * alignment;
    }

    int64_t CAcbHelper::RoundUpToAlignment(int64_t value, int64_t alignment) {
        return (value + alignment - 1) / alignment * alignment;
    }

    int32_t CAcbHelper::RoundUpToAlignment(int32_t value, int32_t alignment) {
        return (value + alignment - 1) / alignment * alignment;
    }

CGSS_NS_END
