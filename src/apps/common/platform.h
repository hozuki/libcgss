#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

int strcmp_ignore_case(const char *s1, const char *s2);
int strncmp_ignore_case(const char *s1, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif
