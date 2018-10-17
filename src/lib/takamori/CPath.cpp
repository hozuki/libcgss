#include <algorithm>
#include "CPath.h"

using namespace std;
using namespace cgss;

string CPath::Combine(const string &basePath, const string &path1) {
    if (basePath.empty()) {
        return path1;
    }

    if (path1.empty()) {
        return basePath;
    }

    const auto lastBaseChar = basePath[basePath.size() - 1];
    const auto firstAppendChar = path1[0];

    if ((lastBaseChar != '/' && lastBaseChar != '\\') && (firstAppendChar != '/' && firstAppendChar != '\\')) {
        return basePath + "/" + path1;
    } else {
        return basePath + path1;
    }
}

string CPath::GetExtension(const string &path) {
    const auto dotPos = path.rfind('.');

    if (dotPos == string::npos) {
        return "";
    }

    if (dotPos == path.size() - 1) {
        return "";
    }

    const auto slashPos = FindLastSlash(path);

    if (slashPos != string::npos) {
        if (dotPos < slashPos) {
            return "";
        }
    }

    // Including the dot
    return path.substr(dotPos);
}

string CPath::GetFileName(const string &path) {
    const auto slashPos = FindLastSlash(path);

    if (slashPos == string::npos) {
        return path;
    }

    if (slashPos == path.size() - 1) {
        return "";
    }

    return path.substr(slashPos + 1u);
}

std::string CPath::GetDirectoryName(const std::string &path) {
    const auto slashPos = FindLastSlash(path);

    if (slashPos == string::npos) {
        return "";
    }

    return path.substr(0, slashPos);
}

string CPath::GetFileNameWithoutExtension(const string &path) {
    const auto fileName = GetFileName(path);

    const auto dotPos = fileName.rfind('.');

    if (dotPos == string::npos) {
        return fileName;
    }

    return fileName.substr(0, dotPos);
}

string::size_type CPath::FindLastSlash(const string &path) {
    const auto forward = path.rfind('/');
    const auto backward = path.rfind('\\');

    if (forward == string::npos) {
        return backward;
    }

    if (backward == string::npos) {
        return forward;
    }

    return max(forward, backward);
}
