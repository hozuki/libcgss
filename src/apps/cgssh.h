#pragma once

#include <stdint.h>

#ifdef __COMPILE_WITH_CGSS_KEYS
static const uint32_t g_CgssKey1 = 0xF27E3B22;
static const uint32_t g_CgssKey2 = 0x00003657;
#else
static const uint32_t g_CgssKey1 = 0;
static const uint32_t g_CgssKey2 = 0;
#endif
