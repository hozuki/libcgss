#include <stdexcept>
#include <algorithm>

#if defined(WIN32) || defined(_WIN32)
#define NOMINMAX
#endif

#include "jni_helper.hpp"

CGSS_NS_BEGIN

    namespace jni {

        namespace java_exceptions {

            const char *IOException = "java/io/IOException";
            const char *IllegalArgumentException = "java/lang/IllegalArgumentException";
            const char *CgssException = "moe/mottomo/cgss/takamori/CgssException";

        }

        void *getNativePtr(JNIEnv *env, jobject object) {
            jclass nativeObjectClass = env->GetObjectClass(object);

            if (!nativeObjectClass) {
                return nullptr;
            }

            jmethodID getter = env->GetMethodID(nativeObjectClass, "getNativePtr", "()J");

            if (getter == nullptr) {
                return nullptr;
            }

            jlong ptrValue = env->CallLongMethod(object, getter);

            auto u = direct_cast<uint64_t>(ptrValue);
            auto intptr = static_cast<uintptr_t>(u);

            return direct_cast<void *>(intptr);
        }

        void setNativePtr(JNIEnv *env, jobject object, const void *ptr) {
            jclass nativeObjectClass = env->GetObjectClass(object);

            if (!nativeObjectClass) {
                return;
            }

            jmethodID setter = env->GetMethodID(nativeObjectClass, "setNativePtr", "(J)V");

            if (setter == nullptr) {
                return;
            }

            auto intptr = direct_cast<uintptr_t>(ptr);
            auto u = static_cast<uint64_t>(intptr);
            auto val = direct_cast<jlong>(u);

            env->CallVoidMethod(object, setter, val);
        }

        jboolean getBooleanField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getBooleanField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "Z");

            if (!fieldID) {
                throw std::runtime_error("jni::getBooleanField");
            }

            auto val = env->GetBooleanField(thiz, fieldID);

            return val;
        }

        jbyte getByteField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getByteField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "B");

            if (!fieldID) {
                throw std::runtime_error("jni::getByteField");
            }

            auto val = env->GetByteField(thiz, fieldID);

            return val;
        }

        jshort getShortField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getShortField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "S");

            if (!fieldID) {
                throw std::runtime_error("jni::getShortField");
            }

            auto val = env->GetByteField(thiz, fieldID);

            return val;
        }

        jint getIntField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getIntField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "I");

            if (!fieldID) {
                throw std::runtime_error("jni::getIntField");
            }

            auto val = env->GetIntField(thiz, fieldID);

            return val;
        }

        jlong getLongField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getLongField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "J");

            if (!fieldID) {
                throw std::runtime_error("jni::getLongField");
            }

            auto val = env->GetLongField(thiz, fieldID);

            return val;
        }

        jfloat getFloatField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getFloatField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "F");

            if (!fieldID) {
                throw std::runtime_error("jni::getFloatField");
            }

            auto val = env->GetFloatField(thiz, fieldID);

            return val;
        }

        jdouble getDoubleField(JNIEnv *env, jobject thiz, const char *name) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getDoubleField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "D");

            if (!fieldID) {
                throw std::runtime_error("jni::getDoubleField");
            }

            auto val = env->GetDoubleField(thiz, fieldID);

            return val;
        }

        jobject getObjectField(JNIEnv *env, jobject thiz, const char *name, const char *fullClassName) {
            auto clsName = "L" + std::string(fullClassName) + ";";

            std::replace(clsName.begin(), clsName.end(), '.', '/');

            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                throw std::runtime_error("jni::getObjectField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, clsName.c_str());

            if (!fieldID) {
                throw std::runtime_error("jni::getObjectField");
            }

            auto val = env->GetObjectField(thiz, fieldID);

            return val;
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jboolean val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "Z");

            if (!fieldID) {
                return;
            }

            env->SetBooleanField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jbyte val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "B");

            if (!fieldID) {
                return;
            }

            env->SetByteField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jshort val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "S");

            if (!fieldID) {
                return;
            }

            env->SetShortField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jint val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "I");

            if (!fieldID) {
                return;
            }

            env->SetIntField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jlong val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "J");

            if (!fieldID) {
                return;
            }

            env->SetLongField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jfloat val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "F");

            if (!fieldID) {
                return;
            }

            env->SetFloatField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, jdouble val) {
            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, "D");

            if (!fieldID) {
                return;
            }

            env->SetDoubleField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *name, const char *fullClassName, jobject val) {
            auto clsName = "L" + std::string(fullClassName) + ";";

            std::replace(clsName.begin(), clsName.end(), '.', '/');

            jclass clazz = env->GetObjectClass(thiz);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, name, clsName.c_str());

            if (!fieldID) {
                return;
            }

            env->SetObjectField(thiz, fieldID, val);
        }

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

        void throwCgssException(JNIEnv *env, const std::string &message, CGSS_OP_RESULT opResult) {
            auto clazz = env->FindClass(jni::java_exceptions::CgssException);
            auto methodId = env->GetMethodID(clazz, "<init>", "(Ljava/lang/String;I)V");

            auto javaStr = ansiToUtf8(env, message);
            auto opResultInt = static_cast<jint>(opResult);

            auto throwable = env->NewObject(clazz, methodId, javaStr, opResultInt);

            env->Throw((jthrowable)throwable);
        }

        void throwCgssException(JNIEnv *env, const CException &exception) {
            throwCgssException(env, exception.GetExceptionMessage(), exception.GetOpResult());
        }

    }

CGSS_NS_END
