#include <string.h>

#include "platform.h"

int strcmp_ignore_case(const char *s1, const char *s2) {
#if defined(WIN32) || defined(_WIN32)
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
    // Visual C++ 2003 or later
    return _stricmp(s1, s2);
#else
    // Visual C++ 6.0 or earlier
    return stricmp(s1, s2);
#endif
#else
    // POSIX-compliant environments
    return strcasecmp(s1, s2);
#endif
}

int strncmp_ignore_case(const char *s1, const char *s2, size_t n) {
#if defined(WIN32) || defined(_WIN32)
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
    // Visual C++ 2003 or later
    return _strnicmp(s1, s2, n);
#else
    // Visual C++ 6.0 or earlier
    return strnicmp(s1, s2, n);
#endif
#else
    // POSIX-compliant environments
    return strncasecmp(s1, s2, n);
#endif
}
