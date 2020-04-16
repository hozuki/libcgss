#ifdef __cplusplus

#include <algorithm>

#include "utils.h"

using namespace std;
using namespace cgss;

// https://stackoverflow.com/a/874160
static bool hasEnding(const string &fullString, const string &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

string common_utils::ReplaceExtension(const string &s, const string &oldExt, const string &newExt) {
    if (s.size() < oldExt.size()) {
        return s;
    }

    auto sl = s;
    auto extl = oldExt;

    // ALERT!
    // Since the only usage here is replacing extensions, and we guarantee that the
    // extensions are in ASCII, and we don't care about chars before the extension,
    // we can use this method (tolower()). Otherwise, it causes trouble for non-
    // ASCII encodings.
    std::transform(sl.begin(), sl.end(), sl.begin(), std::tolower);
    std::transform(extl.begin(), extl.end(), extl.begin(), std::tolower);

    if (!hasEnding(sl, extl)) {
        return s;
    }

    return s.substr(0, s.size() - oldExt.size()) + newExt;
}

string common_utils::ReplaceAnyExtension(const string &s, const string &newExt) {
    auto lastDotPos = s.rfind('.');

    if (lastDotPos == string::npos) {
        return s + newExt;
    } else {
        return s.substr(0, lastDotPos) + newExt;
    }
}

void common_utils::CopyStream(IStream *src, IStream *dst) {
    const size_t bufferSize = 10240;
    uint8_t buffer[bufferSize] = {0};
    uint32_t read = 1;

    while (read > 0) {
        read = src->Read(buffer, bufferSize, 0, bufferSize);

        if (read > 0) {
            dst->Write(buffer, bufferSize, 0, read);
        }

        if (read < bufferSize) {
            break;
        }
    }
}

#endif
