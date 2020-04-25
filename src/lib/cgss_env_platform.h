#pragma once

#if (defined(_WIN64) || defined(__LP64__) || defined(__LLP64__))
#define __CGSS_ARCH_X64__
#else
#define __CGSS_ARCH_X86__
#endif

#if (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))

#define __CGSS_OS_WINDOWS__

#else

#define __CGSS_OS_UNIX__

#endif
