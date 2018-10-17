#include "../takamori/streams/IStream.h"
#include "../takamori/streams/CBinaryReader.h"
#include "../takamori/exceptions/CFormatException.h"
#include "CAcbHelper.h"
#include "CAfs2Archive.h"

using namespace cgss;

static const uint8_t Afs2Signature[] = {0x41, 0x46, 0x53, 0x32}; // 'AFS2'
static const int32_t InvalidCueId = -1;

CAfs2Archive::CAfs2Archive(cgss::IStream *stream, uint64_t offset, const char *fileName, bool_t disposeStream) {
    _stream = stream;
    _streamOffset = offset;
    _disposeStream = disposeStream;

    const auto fileNameLength = strlen(fileName);
    _fileName = new char[fileNameLength + 1];
    memset(_fileName, 0, fileNameLength + 1);
    strncpy(_fileName, fileName, fileNameLength);

    Initialize();
}

CAfs2Archive::~CAfs2Archive() {
    if (_disposeStream) {
        delete _stream;
        _stream = nullptr;
    }

    if (_fileName) {
        delete[] _fileName;
        _fileName = nullptr;
    }
}

bool_t CAfs2Archive::IsAfs2Archive(IStream *stream, uint64_t offset) {
    uint8_t fileSignature[4] = {0};

    auto pos = stream->GetPosition();
    stream->SetPosition(offset);
    CBinaryReader::PeekBytes(stream, fileSignature, 4, 0, 4);
    stream->SetPosition(pos);

    bool_t b = TRUE;

    for (auto i = 0; i < 4; ++i) {
        b = static_cast<bool_t>(b && fileSignature[i] == Afs2Signature[i]);
    }

    return b;
}

void CAfs2Archive::Initialize() {
    auto stream = _stream;
    auto offset = _streamOffset;
    auto acbFileName = _fileName;

    if (!IsAfs2Archive(stream, offset)) {
        throw CFormatException("The file is not a valid AFS2 archive.");
    }

    CBinaryReader reader(stream);

    auto fileCount = reader.PeekInt32LE(offset + 8);

    if (fileCount > 65535) {
        throw CFormatException("File count exceeds max file entries.");
    }

    auto byteAlignment = reader.PeekUInt32LE(offset + 12);
    _byteAlignment = byteAlignment & 0xffff;
    _hcaKeyModifier = static_cast<uint16_t>(byteAlignment >> 16);

    auto version = reader.PeekUInt32LE(offset + 4);
    _version = version;

    auto offsetFieldSize = (version >> 8) & 0xff;
    uint32_t offsetMask = 0;

    for (auto i = 0; i < offsetFieldSize; ++i) {
        offsetMask |= static_cast<uint32_t>(0xff << (i * 8));
    }

    auto prevCueId = InvalidCueId;
    auto fileOffsetFieldBase = 0x10 + fileCount * 2;

    for (uint32_t i = 0; i < fileCount; ++i) {
        auto currentOffsetFieldBase = fileOffsetFieldBase + offsetFieldSize * i;
        AFS2_FILE_RECORD record = {0};

        record.cueId = reader.PeekUInt16LE(offset + (0x10 + 2 * i));
        record.fileOffsetRaw = reader.PeekUInt32LE(offset + currentOffsetFieldBase);

        record.fileOffsetRaw &= offsetMask;
        record.fileOffsetRaw += offset;

        record.fileOffsetAligned = CAcbHelper::RoundUpToAlignment(record.fileOffsetRaw, (uint64_t)GetByteAlignment());

        if (i == fileCount - 1) {
            record.fileSize = reader.PeekUInt32LE(offset + currentOffsetFieldBase + offsetFieldSize) + offset - record.fileOffsetAligned;
        }

        if (prevCueId != InvalidCueId) {
            auto &rec = _files[prevCueId];
            rec.fileSize = record.fileOffsetRaw - rec.fileOffsetAligned;
        }

        _files[record.cueId] = record;

        prevCueId = record.cueId;
    }
}

const std::map<uint32_t, AFS2_FILE_RECORD> &CAfs2Archive::GetFiles() const {
    return _files;
}

uint32_t CAfs2Archive::GetVersion() const {
    return _version;
}

uint32_t CAfs2Archive::GetByteAlignment() const {
    return _byteAlignment;
}

uint16_t CAfs2Archive::GetHcaKeyModifier() const {
    return _hcaKeyModifier;
}

const char *CAfs2Archive::GetFileName() const {
    return _fileName;
}
