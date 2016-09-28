#ifndef CGSS_POLYFILL_H
#define CGSS_POLYFILL_H

#if (defined(_WIN64) || defined(__LP64__) || defined(__LLP64__))
#define __ARCH_X64__
#else
#define __ARCH_X86__
#endif

#if (defined(_WIN32) || defined(__CYGWIN__))

#define __ON_WINDOWS__
#ifndef _MBCS
#define _MBCS
#endif
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#else

#define __ON_UNIX__

#endif

#include "dl.h"
#include "io.h"

#endif //CGSS_POLYFILL_H
