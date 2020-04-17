#pragma once

#include <cstdint>

#include "cgss_jni_ns.h"

CGSS_JNI_NS_BEGIN

        namespace math {

            int32_t computeAvailableBufferSize(int32_t bufferSize, int32_t offset, int32_t count);

        }

CGSS_JNI_NS_END
