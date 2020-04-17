#include <string>
#include <jni.h>
#include <cgss_env.h>

CGSS_NS_BEGIN

    namespace jni {

        char *utf8ToAnsi(const char *str, bool *isCopy);

        char *ansiToUtf8(const char *str, bool *isCopy);

        void freeString(char **str);

        std::string utf8ToAnsi(JNIEnv *env, jstring str);

        jstring ansiToUtf8(JNIEnv *env, const std::string &str);

    }

CGSS_NS_END
