#include <string>
#include <jni.h>
#include <cgss_env.h>

CGSS_NS_BEGIN

    namespace jni {

        std::string utf8ToAnsi(JNIEnv *env, jstring str);

        jstring ansiToUtf8(JNIEnv *env, const std::string &str);

    }

CGSS_NS_END
