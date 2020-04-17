#include <algorithm>

#include "jni_helper_math.h"

CGSS_JNI_NS_BEGIN

        namespace math {

            int32_t computeAvailableBufferSize(int32_t bufferSize, int32_t offset, int32_t count) {
                if (count <= 0) {
                    return 0;
                }

                if (bufferSize <= 0) {
                    return 0;
                }

                if (offset < 0) {
                    return 0;
                }

                if (offset + count > bufferSize) {
                    return std::max<int32_t>(bufferSize - offset, 0);
                } else {
                    return count;
                }
            }

        }

CGSS_JNI_NS_END
