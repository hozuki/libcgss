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

CGSS_NS_END
